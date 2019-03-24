#include "Strategy.h"
#include "Graph.h"
#include <iostream>
#include <set>
#include <QtSql>

using std::set;

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

void Strategy::startStrategy(QString &log)
{
    switch (type) {
        case 1:
            cheapestStrategy(log);
            break;
        case 2:
            fastestStrategy(log);
            break;
        case 3:
            timeLimitStrategy(log);
            break;
    }
}

void Strategy::cheapestStrategy(QString &log)
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    //std::cout << QDir::currentPath().toStdString() << std::endl;
    db.setDatabaseName(QDir::currentPath() + QString("/travel_query.db"));
    if (!db.open())
        log.append("Failed to connect to database\n");
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

    log.append("录入城市：\n");
    for (iter = _set.begin(); iter != _set.end(); iter++) {
        cityList.push_back(*iter);
        log.append(QString::fromStdString(*iter) + " ");
    }
    log.append(QString("\n%1\n").arg(cityList.end() - cityList.begin()));

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
     log.append(QString("出发地: %1    目的地: %2\n").arg(QString::fromStdString(depart)).arg(QString::fromStdString(dest)));
     log.append("旅游线路:\n");
     vector<Path>::iterator iter2;
     MyTime timeUsed;
     MyTime startTime;
     MyTime endTime;
     unsigned short day = 0;
     for (iter2 = result.route.begin(); iter2 != result.route.end(); iter2++) {
        QString start = QString::fromStdString((*iter2).start);
        QString end = QString::fromStdString((*iter2).end);
        log.append(QString("路线: %1--->%2\n").arg(start).arg(end));
        timeUsed = G.getTimeTableValue((*iter2).start, (*iter2).end);
        log.append(QString("花费金额: %1    ").arg((*iter2).moneyCost));
        select = QString("select * from time_table where Dep='%1' and Dest='%2' and Price=%3 order by Time_Cost").arg(start).arg(end).arg((*iter2).moneyCost);
        query.exec(select);
        query.first();
        MyTime period, tempPeriod;
        period.parseString(query.value("Dep_Time").toString());
        while(query.next()) {
            tempPeriod.parseString(query.value("Dep_Time").toString());
            if(tempPeriod < period)
                period = tempPeriod;
        }
        query.first();
        log.append(QString("出发时间: %1时%2分    ").arg(period.hour).arg(period.minute));
        log.append(QString("用时: %1天%2时%3分\n").arg(timeUsed.day).arg(timeUsed.hour).arg(timeUsed.minute));
        if(departTime.day == 0 && departTime.hour == 0 && departTime.minute == 0) {
            departTime = period;
            destTime = departTime + timeUsed;
            day += destTime.day;
            continue;
        }
        while(query.next()) {
            tempPeriod.parseString(query.value("Dep_Time").toString());
            if(!(tempPeriod < destTime))
                period = tempPeriod;
        }
        destTime.day = 0;
        if(destTime > period)
            day += 1;
        destTime = period + timeUsed;
        day += destTime.day;
     }
     destTime.day = day;
     timeUsed = destTime - departTime;
     log.append(QString("到达用时: %1天%2时%3分\n").arg(destTime.day).arg(destTime.hour).arg(destTime.minute));
     log.append(QString("区间用时: %1天%2时%3分\n").arg(timeUsed.day).arg(timeUsed.hour).arg(timeUsed.minute));
     log.append(QString("总花费金额: %1\n").arg(result.moenyCost));
}

void Strategy::fastestStrategy(QString &log)
{

}

void Strategy::timeLimitStrategy(QString &log)
{

}
