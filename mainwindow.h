#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "visitsmodel.h"
#include "visitdelegate.h"
#include "databasemanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DatabaseManager * dbManager, QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_removeButton_clicked();

    void on_addButton_clicked();

//    void slot_table_clicked(int col);

private:
    Ui::MainWindow *ui;
    VisitsModel visitsModel;
    VisitDelegate visitDelegate;
    DatabaseManager * dbManager;
};

#endif // MAINWINDOW_H
