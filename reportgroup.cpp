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

void ReportGroup::insertReport(QString reportName, QString reportGuide, QString reportSql)
{
    Report* newReport = new Report(reportName, reportGuide, reportSql);

    this->reports.append(newReport);
}

ReportGroup::ReportGroup(QString name):tableName(name)
{

}
