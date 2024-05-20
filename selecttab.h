

#pragma once
#include <QtWidgets>
#include <QtSql>
#include "user.h"
#include <unordered_map>
#include "paramsenterwidget.h"
#include "addwindow.h"
#include "editwindow.h"

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

    void editRow(QStringList newValues);
    void deleteRow();

    void emitTableChanged(int n);
    void emitQueryRecalled(int n);
    void emitOpenAddWindow();
    void tableDoubleClicked(const QModelIndex& index);
    void openEditWindow(QStringList values, QStringList columnsNames);


    void insertNewRow(QStringList values);
signals:
    void addWindowButtonClicked(QString currentTableName);
    void newTablePicked(int n,QString newTableName);
    void queryRecalled(int n);
    void editWindowOpen(QStringList values, QStringList columnsNames);


private:
    static std::unordered_map<QString,QString> columnsNameDictionary;

    int currentSelectedRow;

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
    QSqlRecord deletingRow;
    QPushButton* recallQuery;
    void setNewReports(QString name);
    void setNewTable(QString name);
    void initDatabase();
public:

    SelectTab();
};

