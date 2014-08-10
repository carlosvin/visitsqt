#include "databasemanager.h"
#include <QDir>
#include <QSqlQuery>

const char * DatabaseManager::DB_NAME = "visits.sqlite.db";
const char * DatabaseManager::DATE_FORMAT = "yyyy-MM-dd";
const char * DatabaseManager::TABLE_NAME = "visits";

DatabaseManager::DatabaseManager(QObject* parent):QObject(parent)
{

}
DatabaseManager::~DatabaseManager(){}

bool DatabaseManager::openDB()
{
    // Find QSLite driver
    db = QSqlDatabase::addDatabase("QSQLITE");

    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append(DB_NAME);
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
    #else
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    db.setDatabaseName(DB_NAME);
    #endif

    // Open databasee
    return db.open();
}

QSqlError DatabaseManager::lastError()
{
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return db.lastError();
}

bool DatabaseManager::deleteDB()
{
    // Close database
    db.close();

    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append(DB_NAME);
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
    #else

    // Remove created database binary file
    return QFile::remove(DB_NAME);
    #endif
}

bool DatabaseManager::createVisitsTable()
{
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query;
        ret = query.exec(
             "create table visits "
             "(date date primary key, "
             "men integer, "
             "women integer, "
             "boys integer, "
             "girls integer)");

    }
    return ret;
}

bool DatabaseManager::insertVisit(const Visit & visit)
{
    if (db.isOpen())
    {
        QSqlQuery query;
        query.prepare("INSERT INTO visits (date, men, women, boys, girls) "
                           "VALUES (?, ?, ?, ?, ?)");
        query.addBindValue(visit.getDate().toString(DATE_FORMAT));
        for (int i=0; i<Visit::MAX_TYPES; i++)
        {
            query.addBindValue(visit.getVisit(static_cast<VisitType>(i)));
        }
        bool result = query.exec();
        if (result){
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

bool DatabaseManager::getVisit(const QDate & date, Visit & visit)
{

    QSqlQuery query(QString("select * from visits where date = %1").arg(date.toString(DATE_FORMAT)));
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
    QSqlQuery query(QString("select * from visits"));
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

    QSqlQuery query(QString("delete from visits where date = %1").arg(date.toString(DATE_FORMAT)));
    return query.exec();
}
