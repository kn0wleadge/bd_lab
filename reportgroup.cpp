#include "reportgroup.h"


ReportGroup::~ReportGroup()
{
    for(auto &e: reports)
    {
        delete e;
    }
}

QString ReportGroup::getTableName()
{
    return this->tableName;
}

QVector<Report*> ReportGroup::getReports()
{
    return this->reports;
}

void ReportGroup::insertReport(QString reportName, QStringList reportGuide, QString reportSql, QStringList reportParams/* = QStrlingList() */)
{
    Report* newReport = new Report(reportName, reportGuide, reportSql, reportParams);

    this->reports.append(newReport);
}

ReportGroup::ReportGroup(QString name):tableName(name)
{

}
