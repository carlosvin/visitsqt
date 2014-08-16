#include "databasemanager.h"
#include <QDir>
#include <QSqlQuery>

const QString Query::CREATE_TABLE("create table %1 (date date primary key, men integer, women integer, boys integer,girls integer)");
const QString Query::CREATE("INSERT INTO %1 (date, men, women, boys, girls) VALUES (?, ?, ?, ?, ?)");
const QString Query::READ("select * from %1 where date='%2'");
const QString Query::UPDATE("UPDATE %1 set men=?, women=?, boys=?, girls=? WHERE date='%2'");
const QString Query::DELETE("delete from %1 where date='%2'");
const QString Query::READ_ALL("select * from %1");

Query::Query(const char * tableName):
    createTable(Query::CREATE_TABLE.arg(tableName)),
    create(Query::CREATE.arg(tableName)),
    read(Query::READ.arg(tableName)),
    update(Query::UPDATE.arg(tableName)),
    del(Query::DELETE.arg(tableName)),
    readAll(Query::READ_ALL.arg(tableName))
{

}

const char * DatabaseManager::DB_NAME = "visits.sqlite.db";
const char * DatabaseManager::DATE_FORMAT = "yyyy-MM-dd";
const char * DatabaseManager::TABLE_NAME = "visits";

DatabaseManager::DatabaseManager(QObject* parent):
    QObject(parent),
    querySet(DatabaseManager::TABLE_NAME)
{

}
DatabaseManager::~DatabaseManager(){}

bool DatabaseManager::openDB()
{
    // Find QSLite driver
    db = QSqlDatabase::addDatabase("QSQLITE");

    //#ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append(DB_NAME);
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
    //#else
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    //db.setDatabaseName(DB_NAME);
    //#endif

    // Open databasee
    return db.open();
}

QSqlError DatabaseManager::lastError()
{
    return db.lastError();
}

bool DatabaseManager::deleteDB()
{
    // Close database
    db.close();

    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append(TABLE_NAME);
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
    #else

    // Remove created database binary file
    return QFile::remove(DB_NAME);
    #endif
}

bool DatabaseManager::createVisitsTable()
{
    if (db.isOpen())
    {
        return QSqlQuery(querySet.createTable).exec();
    }
    else
    {
        return false;
    }
}

bool DatabaseManager::insertVisit(const Visit & visit)
{
    if (db.isOpen())
    {
        QSqlQuery query;
        query.prepare(querySet.create);
        query.addBindValue(visit.getDate().toString(DATE_FORMAT));
        for (int i=0; i<Visit::MAX_TYPES; i++)
        {
            query.addBindValue(visit.getVisit(static_cast<VisitType>(i)));
        }
        if (query.exec()){
            return true;
        }else{
            qDebug()<< query.lastError();
            return false;
        }
    }
    else
    {
        return false;
    }
}


bool DatabaseManager::updateVisit(const Visit & visit)
{
    if (db.isOpen())
    {
        QSqlQuery query;
        query.prepare(querySet.update.arg(visit.getDate().toString(DATE_FORMAT)));
        for (int i=0; i<Visit::MAX_TYPES; i++)
        {
            query.addBindValue(visit.getVisit(static_cast<VisitType>(i)));
        }
        bool result = query.exec();
        if (result)
        {
            qDebug()<< "updated " <<visit.getDate().toString(DATE_FORMAT);
        }
        else
        {
            qDebug()<< query.lastError();
        }
        return result;
    }
    else
    {
        return false;
    }
}

bool DatabaseManager::getVisit(const QDate & date, Visit & visit)
{

    QSqlQuery query(querySet.read.arg(date.toString(DATE_FORMAT)));
    if (query.next())
    {
        loadDataFromQuery(query, visit);
        return true;
    }
    else
    {
        return false;
    }
}

bool DatabaseManager::loadVisits(QList<Visit> & visits)
{
    QSqlQuery query(querySet.readAll);
    while(query.next()){
        Visit visit;
        loadDataFromQuery(query, visit);
        visits.append(visit);
    }
    return true;
}

void DatabaseManager::loadDataFromQuery(QSqlQuery & query, Visit & visit)
{
    visit.setDate(query.value(0).toDate());
    for (int i = 0; i<Visit::MAX_TYPES; i++)
    {
        visit.setVisit(static_cast<VisitType>(i), query.value(i+1).toInt());
    }
}


bool DatabaseManager::deleteVisit(const QDate & date)
{
    return QSqlQuery(querySet.del.arg(date.toString(DATE_FORMAT))).exec();
}
