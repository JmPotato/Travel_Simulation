#include "Strategy.h"
#include "Graph.h"
#include <iostream>
#include <stack>
#include <set>
#include <QtSql>
#include <algorithm>
using std::set;
using std::stack;
using std::cout;
using std::endl;

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

long findIndex(vector<string> v, string str)
{
    for (long i = 0; i < v.size(); i++)
        if (v[i] == str)
            return i;
    return -1;
}

void Strategy::fastestStrategy(QString &log)
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    //std::cout << QDir::currentPath().toStdString() << std::endl;
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

    for (set<string>::iterator iter = _set.begin(); iter != _set.end(); iter++) {
        cityList.push_back(*iter);
    }

    unsigned long cityNum = cityList.size();
    bool **visited = new bool*[cityNum];
    for(unsigned long i = 0; i < cityNum; i++)
        visited[i]=new bool[cityNum + 1];
    /* 初始化矩阵 */
    for (unsigned long i = 0; i < cityNum; i++)
        for (unsigned long j = 0; j < cityNum + 1; j++)
            visited[i][j] = false;

    long destIndex = findIndex(cityList, dest);
    string temp = cityList[0];
    cityList[0] = dest;
    cityList[destIndex] = temp;
    stack<string> s;
    //s.push(QString::fromStdString(depart));
    s.push(depart);
    visited[findIndex(cityList, depart)][0] = true;
    //QString top;
    string top;
    long n;
    //QString next;
    string next;
    int count = 0;
    //QString tempCity;
    string tempCity;
    stack<string> tempS;    
    vector<string> tempCityPath;
    Path onePath;
    MyTime minEndTime(10000, 10000, 10000);
    while(!s.empty())
    {
        top = s.top();
        if (top == dest)
        {
            MyTime endTime;
            MyTime currentTime;
            tempCityPath.clear();
            tempS = s;
            count++;
            std::cout << count << ": ";
            while (!tempS.empty()) {
                tempCity = tempS.top();
                tempS.pop();
                tempCityPath.push_back(tempCity);
            }
            std::reverse(tempCityPath.begin(), tempCityPath.end());
//            for (auto a : tempCityPath)
//                cout << a << " ";
            //cout << endl;
             //MyTime onePathEndTime;
            for (vector<string>::iterator it = tempCityPath.begin(); it != tempCityPath.end() - 1; it++) {
                currentTime = endTime;
                currentTime.day = 0;
                MyTime minOnePathEndTime(1000, 1000, 1000);
                MyTime onePathStartTime;
                MyTime onePathUsedTime;
                MyTime onePathEndTime;
                bool flag = false;
                query.exec(QString("select * from time_table where Dep='%1' and Dest='%2'").arg(QString::fromStdString(*it)).arg(QString::fromStdString(*(it + 1))));
                while (query.next()) {
                    onePathStartTime.parseString(query.value("Dep_Time").toString());
                    if (currentTime > onePathStartTime)
                        continue;
                    else {
                        flag = true;
                        onePathUsedTime.parseString(query.value("Time_Cost").toString());
                        onePathEndTime = onePathStartTime + onePathUsedTime;
                        if (onePathEndTime < minOnePathEndTime) {
                            minOnePathEndTime = onePathEndTime;
                        }
                    }
                }
                if (!flag)
                    endTime.day += 1;
                else {
                endTime.hour = 0; endTime.minute = 0;
                endTime = endTime + minOnePathEndTime;
                }
            }
            for (auto a : tempCityPath)
                cout << a << " ";
            cout << "用时: ";
            cout << endTime.day << "天" << endTime.hour << "时" << endTime.minute << "分";
            cout << endl;

            if (endTime < minEndTime) {
                minEndTime = endTime;
            }

            s.pop();
            visited[findIndex(cityList, dest)][0] = false;
        }
        else
        {
            for (long i = 0; i < cityList.size(); i++) {
                if (!visited[findIndex(cityList, top)][i + 1] && !visited[i][0]) {
                    n = i;
                    break;
                }
                else
                    n = -1;
            }

            if (n != -1 && s.size() < 4)
            {
               next = cityList[n];
               s.push(next);
               visited[findIndex(cityList, top)][n + 1] = true;
               visited[n][0] = true;
            }
            else
            {
               s.pop();
               for (unsigned long i = 0; i < cityNum + 1; i++)
                 visited[findIndex(cityList, top)][i] = false;
            }
        }
    }

   cout << "最少用时: " << minEndTime.day << "天" << minEndTime.hour << "时" << minEndTime.minute << "分" << endl;
   for(unsigned long i=0; i < cityNum;i++)
       delete[] visited[i];
   delete[] visited;
   cout << count << endl;
   // db.close();
}

void Strategy::timeLimitStrategy(QString &log)
{

}
