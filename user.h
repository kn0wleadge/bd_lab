#ifndef USER_H
#define USER_H
#include <QString>
#include <QStringList>
#include "reportgroup.h"

class User
{
private:
    QString uRole;
    QString userName;
    QStringList availTables;
    QVector<ReportGroup*> availReportGroups;


public:

    User(QString role);
};

#endif // USER_H
