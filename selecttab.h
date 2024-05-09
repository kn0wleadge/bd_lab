

#pragma once
#include <QtWidgets>
#include <QtSql>
#include "user.h"
#include <unordered_map>
#include "paramsenterwidget.h"
#include "addwindow.h"
#include <QThread>
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

    void insertNewRow(QStringList values);
signals:
    void addWindowButtonClicked(QString currentTableName);
    void newTablePicked(int n,QString newTableName);
    void queryRecalled(int n);

private:
    static std::unordered_map<QString,QString> columnsNameDictionary;
    void initColumnsNameDictionary();
    QString rusTableNameToEng(QString cName);
    QString engColumnNameToRus(QString cName);
    // data type for storing information about columns data types
    QVector<std::pair<QString, QStringList>> columnsDataTypes;
    AddWindow* addWindow;

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

