#ifndef ROWSFORDELETEWINDOW_H
#define ROWSFORDELETEWINDOW_H
#include <QtWidgets>
#include <QtSql>
class RowsForDeleteWindow: public QWidget
{
    Q_OBJECT
signals:
    void callDeleteRow();
private slots:
    void emitCallDeleteRow();
private:
    QLabel* label;
    QGridLayout layout;
    QTableWidget* table;
    QPushButton* okButton;
    QPushButton* cancelButton;
public:
    RowsForDeleteWindow(QWidget* parent, QSqlQueryModel* model);
};

#endif // ROWSFORDELETEWINDOW_H
