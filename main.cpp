#include "mainmenu.h"
#include <QApplication>
#include <QtSql>
#include <QtCore>
#include <iostream>

#define N 22

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //qDebug() << QSqlDatabase::drivers();

    mainMenu mm;
    mm.show();

    return a.exec();
}
// int main(int argc, char *argv[]) /* 1 */
// {
//     QVector<float> array;
//     float x = 0;
//     for (int i = 0; i<15;++i)
//     {
//         std::cin>>x;
//         array.push_back(x);
//     }/* 2 */
//     float midA = 0;
//     for (auto& e :array)
//     {
//         midA+=e;
//     }
//     midA/=array.size();/* 3 */
//     float sum = 0;
//     for (auto& e:array)/* 4 */
//     {
//         if (e>midA)/* 5 */
//             sum+=e;/* 6 */
//     }
//     qDebug()<<sum;/* 7 */
//     return 0;
// }
