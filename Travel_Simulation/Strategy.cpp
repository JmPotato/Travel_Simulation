#include "Strategy.h"
#include "Graph.h"
#include <iostream>
#include <set>
#include <QtSql>

using std::set;
using std::cout;
using std::endl;

Strategy::Strategy(int t, string d1, string d2, MyTime departT, MyTime destT)
{
    type = t;
    depart = d1;
    dest = d2;
    departTime = departT;
    destTime = destT;
    result.timeCost.day = 0;
    result.timeCost.hour = 0;
    result.timeCost.minute = 0;
    result.moenyCost = 0;
}

void Strategy::startStrategy()
{
    switch (type) {
        case 1:
            cheapestStrategy();
            break;
        case 2:
            fastestStrategy();
            break;
        case 3:
            timeLimitStrategy();
            break;
    }
}

void Strategy::cheapestStrategy()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    //std::cout << QDir::currentPath().toStdString() << std::endl;
    db.setDatabaseName(QDir::currentPath() + QString("/travel_query.db"));
    if (!db.open())
        std::cerr << "Failed to connect to database" << std::endl;
   // db.exec("select * from time_table");
    QSqlQuery query(db);
    query.exec("select * from time_table");
    vector<string> cityList;
    set<string> _set;
    string str;
    while (query.next()) {
        str = query.value("Dep").toString().toStdString();
        _set.insert(str);
    }
    set<string>::iterator iter;

    cout << "录入城市：" << endl;
    for (iter = _set.begin(); iter != _set.end(); iter++) {
        cityList.push_back(*iter);
        cout << *iter << " ";
    }
    cout << std::endl << cityList.end() - cityList.begin() << endl;

    Graph G(cityList.size());
    //Graph MyTimeGraph
    G.setVexsList(cityList);

    vector<string>::iterator i, j;
    int value;
    QString select;
    for (i = cityList.begin(); i != cityList.end(); i++)
        for (j = cityList.begin(); j != cityList.end(); j++) {
            if (*i == *j);
                //G.setValue(*i, *j, 0);
            else {
                QString s1 = QString::fromStdString(*i);
                QString s2 = QString::fromStdString(*j);
                select = QString("select * from time_table where Dep='%1' and Dest='%2' order by Price asc limit 1").arg(s1).arg(s2);
                query.exec(select);
                query.first();
                value = query.value("Price").toInt();
                G.setValue(*i, *j, value);

                //QString depMyTime = query.value("Dep_Time").toString();
                QString MyTimeCost = query.value("Time_Cost").toString();
                MyTime t1;
                t1.day = 0;
                t1.hour = MyTimeCost.section(":", 0, 0).toUShort();
                t1.minute = MyTimeCost.section(":", 1, 1).toUShort();
                G.setTimeTableValue(*i, *j, t1);
            }
        }
     G.printMatrix();
     G.shortestPathDJ(depart, dest, result);
     cout << "出发地: " << depart << "目的地: " << dest << endl;
     cout << "旅游线路:" << endl;
     vector<Path>::iterator iter2;
     MyTime timeUsed;
     unsigned short day = 0;
     for (iter2 = result.route.begin(); iter2 != result.route.end(); iter2++) {
        cout << (*iter2).start << "--->" << (*iter2).end << "   花费金钱: "<< (*iter2).moneyCost << "   用时: ";
        timeUsed = G.getTimeTableValue((*iter2).start, (*iter2).end);
        cout << timeUsed.hour << "时" << timeUsed.minute << "分" << endl;
        QString start = QString::fromStdString((*iter2).start);
        QString end = QString::fromStdString((*iter2).end);
        select = QString("select * from time_table where Dep='%1' and Dest='%2' and Price=%3 order by Time_Cost asc limit 1").arg(start).arg(end).arg((*iter2).moneyCost);
        query.exec(select);
        query.first();
        cout << query.value("Dep_Time").toString().toStdString() << endl;
        MyTime period;
        period.parseString(query.value("Dep_Time").toString());
        if(departTime.day == 0 && departTime.hour == 0 && departTime.minute == 0) {
            departTime = period;
            destTime = departTime + timeUsed;
            continue;
        }
        destTime.day = 0;
        if(destTime > period)
            day += 1;
        destTime = period + timeUsed;
     }
     destTime.day = day;
     timeUsed = destTime - departTime;
     cout << "总用时: " << timeUsed.day << "天" << timeUsed.hour << "时" << timeUsed.minute << "分" << endl;
     cout << "总花费金钱: " << result.moenyCost << endl;
}

void Strategy::fastestStrategy()
{

}

void Strategy::timeLimitStrategy()
{

}
