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
        newGroup1->insertReport("Запрос на получение информации о банке, где больше всего рыбы определенного вида",
                                "Введите название рыбы",
                                "select  BName as 'Название банки', Cords as 'Местоположение'"
                                "from bank"
                                "where BNum = (select BNum from fish where Quantity = (select max(Quantity) from fish where FishName = 'Carp') and FishName = 'Carp');"
                                );
        newGroup1->insertReport("Запрос на получение суммарного улова за всей рейсы, в которые траулеры вышли в определенный период",
                                ""
                               " select VDate as 'Дата отправления', Retdate as 'Дата прибытия', sum(fishQuantity) as 'Суммарное количество пойманной рыбы' , trawler.TName"
                                   " from voyage, fishCatch, trawler"
                                    "where fishCatch.resNum in (select resNum from fishingSessionRes where DepDate between '2024-03-19' and '2024-05-17' )"
                                        "and voyage.VDate between '2024-03-19' and '2024-05-17'"
                                            "and trawler.TNum = voyage.TNum"
                                      "group by VDate, Retdate, trawler.TName;"

                                )


        availReportGroups.append(newGroup1);
    }
}
