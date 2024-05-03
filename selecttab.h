

#pragma once
#include <QtWidgets>
#include <QtSql>
#include "user.h"
#include "paramsenterwidget.h"
#include "addwindow.h"
//под одну категорию пользователей
class SelectTab:public QWidget
{
    Q_OBJECT
private slots:
    void tableChanged(QString name);
    void reportChanged(int n);
    void executeQuery(QStringList paramsList);
    void openAddWindow(QString currentTableName);

    void emitTableChanged(int n);
    void emitQueryRecalled(int n);
    void emitOpenAddWindow();

signals:
    void addWindowButtonClicked(QString currentTableName);
    void newTablePicked(int n,QString newTableName);
    void queryRecalled(int n);

private:
    QGridLayout selectLayout;
    QTableView* bdView;
    QSqlQueryModel* model;
    QSqlQuery* paramsQuery;

    QLabel tablesSelectLabel;
    QComboBox* tables;
    User* user;
    QLabel searchByLabel;
    QComboBox* reports;
    QWidget* paramsEnter;

    QString queryBuffer;

    ParamsEnterWidget* paramsWindow;

    QSqlTableModel* bdModel;
    static QSqlDatabase db;

    QStringList columnsNames;

    QPushButton* recallQuery;
    void setNewReports(QString name);
    void setNewTable(QString name);
    void initDatabase();
public:

    SelectTab();
};

