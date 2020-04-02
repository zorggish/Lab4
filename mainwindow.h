#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ui_mainwindow.h>
#include <QSqlDatabase>
#include <QtSql>
#include "QStandardItemModel"
#include "QStandardItem"
#include "QMessageBox"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    enum tableType {accounts, notebooks};
    tableType currentTable = accounts;
    int currentAccountRecord = 0, currentNotebookRecord = 0, maxAccountRecond, maxNotebookRecond;

public slots:
    void setAccountsTable()
    {
        /**
         * @brief Переключение на обработку таблицы аккаунтов
         */
        currentTable = accounts;
        ui->firstLabel->setText("Фамилия");
        ui->secondLabel->setText("Имя");
        ui->thirdLabel->setText("Почта");
        db.open();
        QSqlQuery query;
        query.exec("SELECT * FROM Accounts where Id=" + QString::number(currentAccountRecord));
        query.first();
        loadValues(query);
        db.close();
        ui->numberDisplay->setText(QString::number(currentAccountRecord) + "/" + QString::number(maxAccountRecond));
    }

    void setNotebooksTable()
    {
        /**
         * @brief Переключение на обработку таблицы ноутбуков
         */
        currentTable = notebooks;
        ui->firstLabel->setText("Модель");
        ui->secondLabel->setText("Производитель");
        ui->thirdLabel->setText("Процессор");
        db.open();
        QSqlQuery query;
        query.exec("SELECT * FROM Notebooks where Id=" + QString::number(currentNotebookRecord));
        query.first();
        loadValues(query);
        db.close();
        ui->numberDisplay->setText(QString::number(currentNotebookRecord) + "/" + QString::number(maxNotebookRecond));
    }

    void leftButtonClicked()
    {
        /**
         * @brief Переключение на предыдущую запись в таблице
         */
        if(currentTable == accounts)
        {
            if(currentAccountRecord > 0){
                db.open();
                currentAccountRecord--;
                ui->numberDisplay->setText(QString::number(currentAccountRecord) + "/" + QString::number(maxAccountRecond));
                QSqlQuery query;
                query.exec("SELECT * FROM Accounts where Id=" + QString::number(currentAccountRecord));
                query.first();
                loadValues(query);
                db.close();
            }
        }
        else if(currentTable == notebooks)
        {
            if(currentNotebookRecord > 0){
                db.open();
                currentNotebookRecord--;
                ui->numberDisplay->setText(QString::number(currentNotebookRecord) + "/" + QString::number(maxNotebookRecond));
                QSqlQuery query;
                query.exec("SELECT * FROM Notebooks where Id=" + QString::number(currentNotebookRecord));
                query.first();
                loadValues(query);
                db.close();
            }
        }
    }

    void rightButtonClicked()
    {
        /**
         * @brief Переключение на следующую запись в таблице
         */
        if(currentTable == accounts)
        {
            if(currentAccountRecord < maxAccountRecond){
                db.open();
                currentAccountRecord++;
                ui->numberDisplay->setText(QString::number(currentAccountRecord) + "/" + QString::number(maxAccountRecond));
                QSqlQuery query;
                query.exec("SELECT * FROM Accounts");
                query.seek(currentAccountRecord-1);
                loadValues(query);
                db.close();
            }
        }
        else if(currentTable == notebooks)
        {
            if(currentNotebookRecord < maxNotebookRecond){
                db.open();
                currentNotebookRecord++;
                ui->numberDisplay->setText(QString::number(currentNotebookRecord) + "/" + QString::number(maxNotebookRecond));
                QSqlQuery query;
                query.exec("SELECT * FROM Notebooks");
                query.seek(currentNotebookRecord-1);
                loadValues(query);
                db.close();
            }
        }
    }

    void addButtonClicked()
    {
        /**
         * @brief Добавление записи в таблицу
         */
        db.open();
        QSqlQuery query;
        if(currentTable == accounts){

            query.prepare("INSERT INTO Accounts VALUES (:Id, :Surname, :Name, :Mail)");
            query.bindValue(":Id", maxAccountRecond+1);
            query.bindValue(":Surname", ui->firstEdit->text());
            query.bindValue(":Name", ui->secondEdit->text());
            query.bindValue(":Mail", ui->thirdEdit->text());
            query.exec();
            maxAccountRecond++;
            ui->numberDisplay->setText(QString::number(currentAccountRecord) + "/" + QString::number(maxAccountRecond));
            query.exec("SELECT * FROM Accounts where Id=" + QString::number(currentAccountRecord));
            query.first();
            loadValues(query);
        } else if (currentTable == notebooks) {
            query.prepare("INSERT INTO Notebooks VALUES (:Id, :Model, :Vendor, :CPU)");
            query.bindValue(":Id", maxNotebookRecond+1);
            query.bindValue(":Model", ui->firstEdit->text());
            query.bindValue(":Vendor", ui->secondEdit->text());
            query.bindValue(":CPU", ui->thirdEdit->text());
            query.exec();
            maxNotebookRecond++;
            ui->numberDisplay->setText(QString::number(currentNotebookRecord) + "/" + QString::number(maxNotebookRecond));
            query.exec("SELECT * FROM Notebooks where Id=" + QString::number(currentNotebookRecord));
            query.first();
            loadValues(query);
        }
        db.close();
    }

    void saveButtonClicked()
    {
        /**
         * @brief Обновление записи в таблице
         */
        db.open();
        QSqlQuery query;
        if(currentTable == accounts){
            if(currentAccountRecord!=0)
                query.exec("UPDATE Accounts SET Surname='"+ui->firstEdit->text()+"', Name='"+ui->secondEdit->text()+"', Mail='"+ui->thirdEdit->text()+"' WHERE Id=" + QString::number(currentAccountRecord));
        }
        else if (currentTable == notebooks){
            if(currentNotebookRecord!=0)
                query.exec("UPDATE Notebooks SET Model='"+ui->firstEdit->text()+"', Vendor='"+ui->secondEdit->text()+"', CPU='"+ui->thirdEdit->text()+"' WHERE Id=" + QString::number(currentNotebookRecord));
        }
        db.close();
    }

    void deleteButtonClicked()
    {
        /**
         * @brief Удаление записи из таблицы
         */
        db.open();
        QSqlQuery query;
        if(currentTable == accounts)
        {
            if(currentAccountRecord!=0){
                query.exec("DELETE FROM Accounts WHERE Id=" + QString::number(currentAccountRecord));
                query.exec("update Accounts set Id=Id-1 WHERE Id>" + QString::number(currentAccountRecord));
                maxAccountRecond--;
            }
        }
        else if (currentTable == notebooks)
        {
            if(currentNotebookRecord!=0){
                query.exec("DELETE FROM Notebooks WHERE Id=" + QString::number(currentNotebookRecord));
                query.exec("update Notebooks set Id=Id-1 WHERE Id>" + QString::number(currentNotebookRecord));
                maxNotebookRecond--;
            }
        }
        db.close();
        leftButtonClicked();
    }

    void queryButtonClicked()
    {
        /**
         * @brief Пользовательский запрос и вывод его на QTable
         */
        db.open();
        QSqlQuery query;

        if(ui->queryEdit->text().left(6).toLower() != "select")
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Ошибка");
            msgBox.setText("Доступен только SELECT");
            msgBox.exec();
            return;
        }

        query.exec(ui->queryEdit->text());
        QStandardItemModel *model = new QStandardItemModel;
        int row = 0;
        while(query.next()){
            model->setItem(row, 0, new QStandardItem(query.value(0).toString()));
            model->setItem(row, 1, new QStandardItem(query.value(1).toString()));
            model->setItem(row, 2, new QStandardItem(query.value(2).toString()));
            model->setItem(row, 3, new QStandardItem(query.value(3).toString()));
            row++;
        }
        ui->tableView->setModel(model);
        db.close();
    }

    void loadValues(QSqlQuery query)
    {
        /**
         * @brief Загрузка значений из таблицы в интерфейс и буфер
         * @param query Объект sql-запроса с полученными данными
         */
        ui->firstEdit->setText(query.value(1).toString());
        ui->secondEdit->setText(query.value(2).toString());
        ui->thirdEdit->setText(query.value(3).toString());
    }
};
#endif // MAINWINDOW_H
