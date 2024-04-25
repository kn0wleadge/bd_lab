#ifndef SELECTWINDOW_H
#define SELECTWINDOW_H
#include <QtWidgets>
#include <QtSql>
#include "selecttab.h"
#include "user.h"
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
    User* user;
public:
    SelectWindow(User* user,QWidget* parent = nullptr);
};


#endif // SELECTWINDOW_H
