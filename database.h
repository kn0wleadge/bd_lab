#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql>
class Database
{
private:
    QSqlDatabase db;
public:
    Database();
    QSqlDatabase getDb();
};

#endif // DATABASE_H
