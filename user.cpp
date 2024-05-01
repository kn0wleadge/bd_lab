#include "user.h"

QStringList User::getAvailTables() const
{
    return availTables;
}

void User::setAvailTables(const QStringList &newAvailTables)
{
    availTables = newAvailTables;
}

QVector<ReportGroup *> User::getAvailReportGroups() const
{
    return availReportGroups;
}

void User::setAvailReportGroups(const QVector<ReportGroup *> &newAvailReportGroups)
{
    availReportGroups = newAvailReportGroups;
}

QString User::getUserName() const
{
    return userName;
}

void User::setUserName(const QString &newUserName)
{
    userName = newUserName;
}

QString User::getURole() const
{
    return uRole;
}

void User::setURole(const QString &newURole)
{
    uRole = newURole;
}

User::User(QString role)
{
    if (role == "fish catch registrator")
    {
        availTables.append("bank");
        availTables.append("fish");
        availTables.append("fishCatch");
        availTables.append("fishingSessionRes");
        availTables.append("voyage");

        ReportGroup* bankGroup = new ReportGroup("bank");
        bankGroup->insertReport("Вывести информацию о всех банках",QStringList(),"select * from bank;");
        bankGroup->insertReport("Запрос на получение списка членов экипажа с определенного рейса",
                                QStringList() << "Введите номер рейса",
                                "select CNum as 'Номер члена экипажа', CName as 'ФИО' from crew where VNum = :vnum ;",
                                QStringList() << ":vnum"
                                );
        bankGroup->insertReport("Запрос на получение информации о банке, где больше всего рыбы определенного вида",
                                QStringList() << "Введите название рыбы",
                                "select  BName as 'Название банки', Cords as 'Местоположение' "
                                "from bank "
                                "where BNum = (select BNum from fish where Quantity = (select max(Quantity) from fish where FishName = :fishname) and FishName = :fishname);",
                                QStringList() << ":fishname"

                                );
        bankGroup->insertReport("Запрос на получение суммарного улова за всей рейсы, в которые траулеры вышли в определенный период",
                                QStringList() << "Введите дату отплытия" << "Введите дату возвращения ",
                               " select VDate as 'Дата отправления', Retdate as 'Дата прибытия', sum(fishQuantity) as 'Суммарное количество пойманной рыбы' , trawler.TName"
                                   " from voyage, fishCatch, trawler "
                                    "where fishCatch.resNum in (select resNum from fishingSessionRes where DepDate between :vdate and :rdate ) "
                                        "and voyage.VDate between :vdate and :rdate "
                                            "and trawler.TNum = voyage.TNum "
                                      "group by VDate, Retdate, trawler.TName;",
                                QStringList() << ":vdate" << ":rdate"

                                );
        bankGroup->insertReport("Запрос на самую получение информации о рыбе, которая распространена больше всего в определенной банке",
                                QStringList() << "Введите название банки",
                                "select FishName as 'Название рыбы',Quantity as 'Количество рыбы', BName as'Название банки' "
                                "from fish, bank "
                                    "where fish.BNum = bank.BNum "
                                      "and bank.BName = :bname "
                                      "and fish.Quantity = (select max(Quantity) from fish where fish.BNum = (select bank.BNum where BName = :bname)); ",
                                QStringList() << ":bname"
                                );
        bankGroup->insertReport("Запрос на получение информации об общем количестве выловленной щуки за все рейсы",
                                QStringList(),
                                "select FishName as 'Название рыбы', sum(fishQuantity) as 'Количество выловленных особей за все время' "
                            "        where FishName = :fishname;",
                                QStringList() << ":fishname"
                                );
        // bankGroup->insertReport("Запрос на получение информации о всей рыбе пойманной в рамках одного рейса",

            //                         )

        ReportGroup* fishGroup = new ReportGroup("fish");
        fishGroup->insertReport("Вывести информацию о всей рыбе",QStringList(),"select * from fish;");
        fishGroup->insertReport("Запрос на получение списка членов экипажа с определенного рейса",
                                QStringList() << "Введите номер рейса",
                                "select CNum as 'Номер члена экипажа', CName as 'ФИО' from crew where VNum = 3 ;"
                                );
        fishGroup->insertReport("Запрос на получение информации о банке, где больше всего рыбы определенного вида",
                                QStringList() << "Введите название рыбы",
                                "select  BName as 'Название банки', Cords as 'Местоположение' "
                                "from bank "
                                "where BNum = (select BNum from fish where Quantity = (select max(Quantity) from fish where FishName = 'Carp') and FishName = 'Carp');"
                                );
        fishGroup->insertReport("Запрос на получение суммарного улова за всей рейсы, в которые траулеры вышли в определенный период",
                                QStringList() << "Введите дату отплытия" << "Введите дату ",
                                " select VDate as 'Дата отправления', Retdate as 'Дата прибытия', sum(fishQuantity) as 'Суммарное количество пойманной рыбы' , trawler.TName"
                                " from voyage, fishCatch, trawler "
                                "where fishCatch.resNum in (select resNum from fishingSessionRes where DepDate between '2024-03-19' and '2024-05-17' ) "
                                "and voyage.VDate between '2024-03-19' and '2024-05-17' "
                                "and trawler.TNum = voyage.TNum "
                                "group by VDate, Retdate, trawler.TName;"

                                );
        fishGroup->insertReport("Запрос на самую получение информации о рыбе, которая распространена больше всего в определенной банке",
                                QStringList() << "Введите название банки",
                                "select FishName as 'Название рыбы', max(Quantity) as 'Количество особей', BName as'Название банки' "
                                " from fish natural join bank "
                                "     where BName = 'Big Fish bank';"
                                );
        fishGroup->insertReport("Запрос на получение информации об общем количестве выловленной щуки за все рейсы",
                                QStringList(),
                                "select FishName as 'Название рыбы', sum(fishQuantity) as 'Количество выловленных особей за все время' "
                                "        where FishName = 'Pike';"
                                );
        ReportGroup* fishCatchGroup = new ReportGroup("fishCatch");
        fishCatchGroup->insertReport("Вывести всю таблицу уловов",
                                     QStringList(),
                                     "select * from fishCatch;"
                                     );

        ReportGroup* fishingSessionResGroup = new ReportGroup("fishingSessionRes");
        fishingSessionResGroup->insertReport("Вывести всю таблицу промежуточных рыбаловных сессий",
                                     QStringList(),
                                     "select * from fishingSessionRes;"
                                     );
        ReportGroup* voyageGroup = new ReportGroup("voyage");
        voyageGroup->insertReport("Вывести всю таблицу рейсов",
                                          QStringList(),
                                          "select * from voyage;"
                                          );

        availReportGroups.append(bankGroup);
        availReportGroups.append(fishGroup);
        availReportGroups.append(fishCatchGroup);
        availReportGroups.append(fishingSessionResGroup);
        availReportGroups.append(voyageGroup);
    }
}
