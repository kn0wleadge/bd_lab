#ifndef REPORT_H
#define REPORT_H
#include <QString>
#include <QStringList>
class Report
{
private:
    QStringList paramsNameList;
    QStringList reportGuide;
    QString reportText;
    QString reportSql;
    Report();
public:
    ~Report();
    QStringList getParamsNameList();
    QStringList getGuide();
    QString getText();
    QString getSql();
    Report(QString text, QStringList guide, QString sql, QStringList params = QStringList());
};

#endif // REPORT_H
