#ifndef REPORT_H
#define REPORT_H
#include <QString>
#include <QStringList>
class Report
{
private:
    QStringList reportGuide;
    QString reportText;
    QString reportSql;
    Report();
public:
    ~Report();
    QStringList getGuide();
    QString getText();
    QString getSql();
    Report(QString text, QStringList guide, QString sql);
};

#endif // REPORT_H
