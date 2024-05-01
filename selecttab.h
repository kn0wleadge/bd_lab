

#pragma once
#include <QtWidgets>
#include <QtSql>
#include "user.h"
#include "paramsenterwidget.h"
//под одну категорию пользователей
class SelectTab:public QWidget
{
    Q_OBJECT
private slots:
    void tableChanged(QString name);
    void reportChanged(int n);
    void emitTableChanged(int n);
    void executeQuery(QStringList paramsList);
    void emitQueryRecalled(int n);

signals:
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

