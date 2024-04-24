#ifndef REPORT_H
#define REPORT_H
#include <QString>
class Report
{
private:
    QString reportGuide;
    QString reportText;
    QString reportSql;
    Report();
public:
    QString getGuide();
    QString getText();
    QString getSql();
    Report(QString guide, QString text, QString sql);
};

#endif // REPORT_H
