#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include "visit.h"

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
    bool getVisit(const QDate & date, Visit & visit);
    bool loadVisits(QList<Visit> & visits);
    bool deleteVisit(const QDate & date);

    QSqlError lastError();

private:
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
