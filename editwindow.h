#ifndef EDITWINDOW_H
#define EDITWINDOW_H
#include <QtWidgets>
#include "rowsfordeletewindow.h"
class EditWindow: public QWidget
{
    Q_OBJECT
signals:
    void changesSubmited(QStringList list);
    void rowDeleteCalled();
    void callDeleteRow();
private slots:
    void emitCallDeleteRow();
    void someValueChanged();
    void cancelChanges();
    void submitChanges();
    void deleteRow();
    void callDeleteWindow();
private:
    QSqlQueryModel* model;
    QGridLayout* mainLayout;
    bool isValueChanged;
    QList<QLineEdit*> valuesList;

public:
    EditWindow(QStringList values,QStringList columnsNames, QSqlQueryModel* bdModel);
};

#endif // EDITWINDOW_H
