#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(DatabaseManager * dbManager, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    visitsModel(dbManager, parent)
{
    this->dbManager = dbManager;

    ui->setupUi(this);
    ui->tableView->setModel(&visitsModel);
    ui->tableView->setItemDelegate(&visitDelegate);

//    connect(ui->tableView->horizontalHeader(), SIGNAL(sectionClicked(int)),
//            this, SLOT(slot_table_clicked(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_removeButton_clicked()
{
    qDebug()<< "remove";
    visitsModel.removeRow(0);
}

void MainWindow::on_addButton_clicked()
{
    visitsModel.insertRow(0);
    qDebug()<< "add";

}

//void MainWindow::slot_table_clicked(int col){
//    qDebug() << "sort col " << col;

//}
