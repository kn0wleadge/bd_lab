#ifndef PARAMSENTERWIDGET_H
#define PARAMSENTERWIDGET_H
#include <QtWidgets>
class ParamsEnterWidget:public QWidget
{

    Q_OBJECT
signals:
    void paramsEntered(QStringList stringList);
protected:
    virtual void closeEvent(QCloseEvent* event) override;
private:
    void emitParamsEntered();
    QGridLayout layout;
    QPushButton* okButton;
    QVector<QLineEdit*> enterFields;
public:
    ParamsEnterWidget(QStringList guideText);
};

#endif // PARAMSENTERWIDGET_H
