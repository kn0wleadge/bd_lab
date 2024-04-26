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

    QStringList getAvailTables() const;
    void setAvailTables(const QStringList &newAvailTables);

    QVector<ReportGroup *> getAvailReportGroups() const;
    void setAvailReportGroups(const QVector<ReportGroup *> &newAvailReportGroups);

    QString getUserName() const;
    void setUserName(const QString &newUserName);

    QString getURole() const;
    void setURole(const QString &newURole);
};

#endif // USER_H
