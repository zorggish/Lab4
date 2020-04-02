#include "mainwindow.h"
#include <ui_mainwindow.h>
#include <QtSql>
#include <QSqlDatabase>
#include <QFile>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect( ui->accountsRadio, &QRadioButton::toggled, this, &MainWindow::setAccountsTable);
    connect( ui->notebooksRadio, &QRadioButton::toggled, this, &MainWindow::setNotebooksTable);
    connect( ui->leftButton, &QRadioButton::clicked, this, &MainWindow::leftButtonClicked);
    connect( ui->rightButton, &QRadioButton::clicked, this, &MainWindow::rightButtonClicked);
    connect( ui->addButton, &QRadioButton::clicked, this, &MainWindow::addButtonClicked);
    connect( ui->saveButton, &QRadioButton::clicked, this, &MainWindow::saveButtonClicked);
    connect( ui->deleteButton, &QRadioButton::clicked, this, &MainWindow::deleteButtonClicked);
    connect( ui->queryButton, &QRadioButton::clicked, this, &MainWindow::queryButtonClicked);

    QString path = QCoreApplication::applicationDirPath() + "/db.db";
    QFile file(path);
    db.setDatabaseName(path);
    if (!file.exists())
    {
        db.open();
        QSqlQuery query;
        query.exec("CREATE TABLE 'Accounts' ('Id' INTEGER, 'Surname' TEXT, 'Name' TEXT, 'Mail' TEXT)");
        query.exec("CREATE TABLE 'Notebooks' ('Id' INTEGER, 'Model' TEXT, 'Vendor' TEXT, 'CPU' TEXT)");
        db.close();
        maxAccountRecond=0;
        maxNotebookRecond=0;
    }
    else
    {
        db.open();
        QSqlQuery query;
        query.exec("SELECT COUNT(Surname) FROM Accounts");
        query.first();
        maxAccountRecond = query.value(0).toInt();

        query.exec("SELECT COUNT(Model) FROM Notebooks");
        query.first();
        maxNotebookRecond = query.value(0).toInt();
        db.close();
    }

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->numberDisplay->setText(QString::number(currentAccountRecord) + "/" + QString::number(maxAccountRecond));
}

MainWindow::~MainWindow()
{
    delete ui;
}

