#ifndef USER_H
#define USER_H
#include <QString>
enum role{fishCatchRegistrator,admin};
class User
{
private:
    role uRole;
    QString userName;

public:

    User();
};

#endif // USER_H
