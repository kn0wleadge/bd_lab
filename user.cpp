#include "user.h"

User::User(QString role)
{
    if (role == "fish catch registrator")
    {
        availTables.append("bank");
        availTables.append("fish");
        availTables.append("fishCatch");
        availTables.append("fishingSessionRes");
        availTables.append("voyage");

        ReportGroup* newGroup1 = new ReportGroup("bank");
        newGroup1->insertReport("Запрос на получение списка членов экипажа с определенного рейса",
                                "Введите номер рейса",
                                "select CNum as 'Номер члена экипажа', CName as 'ФИО' from crew where VNum = 3 ;"
                                );

        availReportGroups.append(newGroup1);
    }
}
