#include "Strategy.h"
#include "Graph.h"
#include <iostream>
#include <set>
#include <QtSql>

using std::set;

Strategy::Strategy(int t, string d1, string d2, MyTime expectedDepartT, MyTime expectedDestT)
{
    type = t;
    depart = d1;
    dest = d2;
    departTime = expectedDepartT;
    expectedDepartTime = expectedDepartT;
    expectedDestTime = expectedDestT;
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
        if(destTime.day == 0 && destTime.hour == 0 && destTime.minute == 0) {
            MyTime earliest = period;
            while(departTime > period) {
                tempPeriod.parseString(query.value("Dep_Time").toString());
                if(tempPeriod < period)
                    period = tempPeriod;
                if(!query.next())
                    break;
            }
            query.first();
            if(departTime > period) {
                departTime = earliest;
                departTime.day += 1;
            } else {
                departTime = period;
            }
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
     timeUsed = destTime - expectedDepartTime;
     log.append(QString("区间用时: %1天%2时%3分\n").arg(timeUsed.day).arg(timeUsed.hour).arg(timeUsed.minute));
     log.append(QString("总花费金额: %1\n").arg(result.moenyCost));
     db.close();
}

void Strategy::fastestStrategy(QString &log)
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + QString("/travel_query.db"));
    if (!db.open())
        log.append("Failed to connect to database\n");
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
    G.setVexsList(cityList);

    //找出所有路径
    G.findAllPath(QString::fromStdString(depart),QString::fromStdString(dest));

    //处理G.allPath 类型为vector<vector<QString>>
    Result best_route;
    MyTime fastTime(1000,1000,1000);  //初始化一个很大的值
    for(vector<vector<QString>>::iterator iter=G.allPath.begin();iter!=G.allPath.end();iter++)
    {
        //每一条路径分为几个Path
        //每一条路径的Path组合起来算出形成Paths
        //选出best_route输出
        vector<QString> oneRoute=*iter;
        vector<QString>::iterator city1;
        vector<QString>::iterator city2;
        int count=0;
        vector<Path> paths;
        MyTime previousArrTime(0,0,0);  //上一班车的到达时间
        MyTime curRouteTime;

        //找出对应一条路线的最快的paths
        for(city1=oneRoute.begin(),city2=oneRoute.begin()++;city2!=oneRoute.end();city1++,city2++)
        {
           Path fastPath;
           MyTime pathDepTime;
           MyTime pathCostTime;
           MyTime pathArrTime;
           QString pathNumber;

           MyTime tmpDepTime;
           MyTime tmpCostTime;
           if(count==0)//第一个Path
           {
               QString select = QString("select * from time_table where Dep='%1' and Dest='%2'").arg(*city1).arg(*city2);
               query.exec(select);
               query.first();
               pathDepTime.parseString(query.value("Dep_Time").toString());
               pathCostTime.parseString(query.value("Time_cost").toString());
               pathArrTime=pathDepTime+pathCostTime;
               pathNumber=query.value("Number").toString();

               while (query.next())
               {
                   tmpDepTime.parseString(query.value("Dep_Time").toString());
                   tmpCostTime.parseString(query.value("Time_cost").toString());
                   if(tmpDepTime+tmpCostTime<pathArrTime)
                   {
                       pathDepTime=tmpDepTime;
                       pathCostTime=tmpCostTime;
                       pathArrTime=pathDepTime+pathCostTime;
                       pathNumber=query.value("Number").toString();
                   }
               }
               fastPath.startTime=pathDepTime;
               fastPath.timeCost=pathCostTime;
               fastPath.endTime=pathArrTime;
               fastPath.number=pathNumber;
               paths.push_back(fastPath);
               previousArrTime=fastPath.endTime;
               count++;
           }
           else  //第二条以后的path
           {
               QString select = QString("select * from time_table where Dep='%1' and Dest='%2'").arg(*city1).arg(*city2);
               query.exec(select);
               //给path赋初值
               while(query.next())
               {
                   tmpDepTime.parseString(query.value("Dep_Time").toString());
                   if(tmpDepTime>previousArrTime)
                   break;
               }
               pathDepTime.parseString(query.value("Dep_Time").toString());
               pathCostTime.parseString(query.value("Time_cost").toString());
               pathArrTime=pathDepTime+pathCostTime;
               pathNumber=query.value("Number").toString();
               //找最早到达city2的path
               while (query.next())
               {
                   tmpDepTime.parseString(query.value("Dep_Time").toString());
                   tmpCostTime.parseString(query.value("Time_cost").toString());
                   if(tmpDepTime>previousArrTime && (tmpDepTime+tmpCostTime<pathArrTime) )
                   {
                       pathDepTime.parseString(query.value("Dep_Time").toString());
                       pathCostTime.parseString(query.value("Time_cost").toString());
                       pathArrTime=pathDepTime+pathCostTime;
                       pathNumber=query.value("Number").toString();
                   }
               }
               fastPath.startTime=pathDepTime;
               fastPath.timeCost=pathCostTime;
               fastPath.endTime=pathArrTime;
               fastPath.number=pathNumber;
               paths.push_back(fastPath);
               previousArrTime=fastPath.endTime;
               count++;
           }
        }

        vector<Path>::iterator first=paths.begin();
        vector<Path>::iterator last=paths.end();
        curRouteTime=(*last).endTime - (*first).startTime;
        if(curRouteTime<fastTime)
        {
           best_route.route=paths;
           fastTime=curRouteTime;
        }
    }
    //找到时间最短的路径,包括了各个path的startTime，TimeCost,endTime,航班号number;
    best_route.timeCost=fastTime;

    //输出best_route
   /* for(vector<Path>::iterator iter=best_route.route.begin();iter!=best_route.route.end();iter++)
        qDebug()<<QString::fromStdString(*(iter)->start)*/


}

void Strategy::timeLimitStrategy(QString &log)
{

}
