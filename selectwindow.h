#ifndef SELECTWINDOW_H
#define SELECTWINDOW_H
#include <QtWidgets>
#include <QtSql>
#include "selecttab.h"
class SelectWindow:public QTabWidget
{
    Q_OBJECT
private slots:
    void createNewTab(int n);
    void closeTab(int n);
public slots:

    void tablesNameChanged(int n,QString newTableName);
private:
    void initTab(int n);
public:
    SelectWindow(QWidget* parent = nullptr);
};


#endif // SELECTWINDOW_H
