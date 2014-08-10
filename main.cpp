#include "databasemanager.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DatabaseManager dbManager;
    if (dbManager.openDB()){
        dbManager.createVisitsTable();
        MainWindow w(&dbManager);
        w.show();
        return a.exec();
    }else{
        qDebug() << "cannot open db";
        return -1;
    }

}
