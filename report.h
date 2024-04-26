#ifndef REPORT_H
#define REPORT_H
#include <QString>
class Report
{
private:
    QString reportGuide;
    QString reportText;
    QString reportSql;
    int paramsQ;
    Report();
public:
    ~Report();
    QString getGuide();
    QString getText();
    QString getSql();
    Report(QString text, QString guide, QString sql);
};

#endif // REPORT_H
