#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include "visit.h"

class Query {
private:
    static const QString CREATE_TABLE;
    static const QString CREATE;
    static const QString READ;
    static const QString UPDATE;
    static const QString DELETE;
    static const QString READ_ALL;

public:
    const QString createTable;
    const QString create;
    const QString read;
    const QString update;
    const QString del;
    const QString readAll;

    Query(const char * tableName);
};

class DatabaseManager : public QObject
{
public:
    static const char * DB_NAME;
    static const char * TABLE_NAME;
    static const char * DATE_FORMAT;
    DatabaseManager(QObject *parent = 0);
    ~DatabaseManager();

    void loadDataFromQuery(QSqlQuery & query, Visit & visit);

public:
    bool openDB();
    bool deleteDB();
    bool createVisitsTable();
    bool insertVisit(const Visit & visit);
    bool updateVisit(const Visit & visit);
    bool getVisit(const QDate & date, Visit & visit);
    bool loadVisits(QList<Visit> & visits);
    bool deleteVisit(const QDate & date);

    QSqlError lastError();

private:
    const Query querySet;
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
