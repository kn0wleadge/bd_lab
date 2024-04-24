#ifndef REPORTGROUP_H
#define REPORTGROUP_H
#include <QString>
#include <QVector>
#include "report.h"
class ReportGroup
{
private:
    QString tableName;
    QVector<Report> reports;

public:
    ReportGroup();
};

#endif // REPORTGROUP_H
