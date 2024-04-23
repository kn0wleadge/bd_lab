

#pragma once
#include <QtWidgets>
#include <QtSql>
//под одну категорию пользователей
class SelectWindow;
class SelectTab:public QWidget
{
    Q_OBJECT
private slots:
    void tableChanged(int n);
    void emitTableChanged(int n);
signals:
    void newTablePicked(int n,QString newTableName);
private:
    QGridLayout selectLayout;
    QTableView* bdView;

    QLabel tablesSelectLabel;
    QComboBox* tables;

    QLabel searchByLabel;
    QComboBox* headers;
    QLineEdit* searchValue;

    QSqlTableModel* bdModel;
    static QSqlDatabase db;

    QStringList columnsNames;

    void setNewHeaders(int n = 0);
    void setNewTable(int n = 0);
    void initDatabase();
public:

    SelectTab();
};
