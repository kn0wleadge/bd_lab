#ifndef ADDWINDOW_H
#define ADDWINDOW_H
#include <QtWidgets>
class AddWindow:public QWidget
{
    Q_OBJECT
signals:
    void valuesEntered(QStringList);
protected:
    virtual void closeEvent(QCloseEvent* event) override;
private:
    QGridLayout layout;
    QPushButton okButton;
    QVector<QLineEdit* > enterFields;
    void emitValuesEntered();
public:
    AddWindow(QStringList params);
};

#endif // ADDWINDOW_H
