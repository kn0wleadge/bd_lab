#ifndef REPORTGROUP_H
#define REPORTGROUP_H
#include <QString>
#include <QVector>
#include "report.h"
class ReportGroup
{
private:
    QString tableName;
    QVector<Report*> reports;

public:
    ~ReportGroup();
    QString getTableName();
    QVector<Report*> getReports();
    void insertReport(QString reportName, QStringList reportGuide, QString reportSql, QStringList reportParams = QStringList());
    ReportGroup(QString name);

};

#endif // REPORTGROUP_H
