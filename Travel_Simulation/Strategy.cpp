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

/**
 * @brief Strategy::Strategy
 * @param t
 * @param d1
 * @param d2
 * @param expectedDepartT
 * @param expectedDestT
 * @author hzy
 */
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

/**
 * @brief Strategy::startStrategy
 * @param log
 * @authors hzy ghz
 *
 * 选择策略并开始策略
 */
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

/**
 * @brief Strategy::cheapestStrategy
 * @param log
 * @authors hzy ghz
 *
 * 策略一算法函数
 */
void Strategy::cheapestStrategy(QString &log)
{
    /* 连接数据库 */
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + QString("/travel_query.db"));
    if (!db.open())
        log.append("Failed to connect to database\n");

    /* 配置接口 */
    QSqlQuery query(db);
    query.exec("select * from time_table");

    /* 读取数据库，读入所有要用的城市 */
    vector<string> cityList;
    set<string> _set;
    string str;
    while (query.next()) {
        str = query.value("Dep").toString().toStdString();
        _set.insert(str);
    }
    set<string>::iterator iter;
    for (iter = _set.begin(); iter != _set.end(); iter++) {
        cityList.push_back(*iter);
    }

    /* 建立算法所需要的 Graph 类 */
    Graph G(cityList.size());
    G.setVexsList(cityList);

    /* 产生调试信息：
     * 打印算法所需要的以花费最少为权值的邻接矩阵
     */
    vector<string>::iterator i, j;
    int value;
    QString select;
    for (i = cityList.begin(); i != cityList.end(); i++)
        for (j = cityList.begin(); j != cityList.end(); j++) {
            if (*i == *j);
            else {
                QString s1 = QString::fromStdString(*i);
                QString s2 = QString::fromStdString(*j);
                select = QString("select * from time_table where Dep='%1' and Dest='%2' order by Price asc limit 1").arg(s1).arg(s2);
                query.exec(select);
                query.first();
                value = query.value("Price").toInt();
                G.setValue(*i, *j, value);
                QString MyTimeCost = query.value("Time_Cost").toString();
                MyTime t1;
                t1.day = 0;
                t1.hour = MyTimeCost.section(":", 0, 0).toUShort();
                t1.minute = MyTimeCost.section(":", 1, 1).toUShort();
                G.setTimeTableValue(*i, *j, t1);
            }
        }
     G.printMatrix();
    /* ----------------------------------------------------------------------------- */

     G.shortestPathDJ(depart, dest, result);    // 调用算法

     /* 产生交互界面所需要的结果信息 */
     log.append(QString("出发地: %1    目的地: %2\n\n").arg(QString::fromStdString(depart)).arg(QString::fromStdString(dest)));
     log.append("旅游线路:\n");
     vector<Path>::iterator iter2;
     MyTime timeUsed;
     unsigned short day = 0;
     /* 按照刚才算法产生的路径重新遍历，得出完整的全部需要的信息 */
     for (iter2 = result.route.begin(); iter2 != result.route.end(); iter2++) {
        QString start = QString::fromStdString((*iter2).start);
        QString end = QString::fromStdString((*iter2).end);
        timeUsed = G.getTimeTableValue((*iter2).start, (*iter2).end);
        select = QString("select * from time_table where Dep='%1' and Dest='%2' and Price=%3 order by Time_Cost").arg(start).arg(end).arg((*iter2).moneyCost);
        query.exec(select);
        query.first();
        MyTime period, tempPeriod;
        QString codeNumber, methodTool;
        period.parseString(query.value("Dep_Time").toString());
        codeNumber = query.value("Number").toString();
        methodTool = query.value("Tran").toString();
        while(query.next()) {
            tempPeriod.parseString(query.value("Dep_Time").toString());
            if(tempPeriod < period) {
                period = tempPeriod;
                codeNumber = query.value("Number").toString();
                methodTool = query.value("Tran").toString();
            }
        }
        if(start == QString::fromStdString(depart)) {
            MyTime earliest = period;
            query.first();
            while(departTime > period) {
                tempPeriod.parseString(query.value("Dep_Time").toString());
                if(tempPeriod > period)
                    period = tempPeriod;
                if(!query.next())
                    break;
            }
            if(departTime > period) {
                departTime = earliest;
                period = earliest;
                departTime.day += 1;
            } else {
                departTime = period;
            }
            destTime = departTime + timeUsed;
            day += destTime.day;
        } else {
            query.first();
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
        log.append(QString("路线: %1--->%2\n").arg(start).arg(end));
        log.append(QString("交通工具: %1    ").arg(methodTool));
        log.append(QString("车次/航班号: %1\n").arg(codeNumber));
        log.append(QString("花费金额: %1\n").arg((*iter2).moneyCost));
        log.append(QString("出发时间: %1时%2分    ").arg(period.hour).arg(period.minute));
        log.append(QString("用时: %1天%2时%3分\n\n").arg(timeUsed.day).arg(timeUsed.hour).arg(timeUsed.minute));
     }
     destTime.day = day;
     timeUsed = destTime - expectedDepartTime;
     log.append(QString("区间用时: %1天%2时%3分\n").arg(timeUsed.day).arg(timeUsed.hour).arg(timeUsed.minute));
     log.append(QString("总花费金额: %1\n").arg(result.moenyCost));
     // 输出结束

     /* 关闭数据库 */
     db.close();
}

/**
 * @brief findIndex
 * @param v
 * @param str
 * @return v 中 str 的序号
 * @author hzy
 *
 * 策略二算法需要的辅助函数
 */
long findIndex(vector<string> v, string str)
{
    for (long i = 0; i < v.size(); i++)
        if (v[i] == str)
            return i;
    return -1;
}

/**
 * @brief Strategy::fastestStrategy
 * @param log
 * @author hzy ghz hyd
 *
 * 策略二算法函数
 * 采用深度优先搜索算法结合回溯算法，遍历所有有效路径（长度 < 4）
 * 从中选出用时最小的路径
 */
void Strategy::fastestStrategy(QString &log)
{
    /* 连接数据库 */
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + QString("/travel_query.db"));
    if (!db.open())
        log.append("Failed to connect to database\n");

    /* 配置数据库接口 */
    QSqlQuery query(db);
    query.exec("select * from time_table");
    vector<string> cityList;

    /* 同策略一，获取城市表 */
    set<string> _set;
    string str;
    while (query.next()) {
        str = query.value("Dep").toString().toStdString();
        _set.insert(str);
    }
    for (set<string>::iterator iter = _set.begin(); iter != _set.end(); iter++) {
        cityList.push_back(*iter);
    }

    /* 策略二算法需要的一些数据结构 */
    stack<string> s;                // 策略二算法运行时记录路径的栈
    string top;                     // 栈顶元素
    long n;                         // 临时存储序号
    string next;                    // 需要访问的下一个元素
    int count = 0;                  // 记录找到的所有路径的个数
    string tempCity;                // 临时存储元素
    stack<string> tempS;            // 临时存储栈，便于提取栈中元素且不破坏原栈内容
    vector<string> tempCityPath;    // 临时存储找到的路径
    vector<string> bestPath;        // 存储找到的最佳路径
    MyTime minEndTime(10000, 10000, 10000); // 存储迭代过程中的最小到达时间
    unsigned long cityNum = cityList.size();// 存储城市个数
    bool **visited = new bool*[cityNum];    // 访问辅助数组，visited[i][j] 表示：
                                            // j = 0: i 是否被访问过
                                            // j != 0 : 访问 i 之后 j 是否被访问郭
    /* 为访问辅助矩阵分配内存 */
    for(unsigned long i = 0; i < cityNum; i++)
        visited[i] = new bool[cityNum + 1];
    /* 初始化访问辅助矩阵 */
    for (unsigned long i = 0; i < cityNum; i++)
        for (unsigned long j = 0; j < cityNum + 1; j++)
            visited[i][j] = false;

    /* 提高算法的运算效率，将终点放到前面来，以便先行入栈 */
    long destIndex = findIndex(cityList, dest);
    string temp = cityList[0];
    cityList[0] = dest;
    cityList[destIndex] = temp;

    /* 初始化 result */
    result.route.clear();
    result.timeCost.day = 0;
    result.timeCost.hour = 0;
    result.timeCost.minute = 0;

    /* 算法运算部分 */
    s.push(depart);                                 // 出发地先行入栈
    visited[findIndex(cityList, depart)][0] = true; // 已访问出发地
    /* 搜索所有路径，并根据进行情况回溯 */
    while(!s.empty())
    {
        top = s.top();

        /* 找到了一条从起点到终点的路径 */
        if (top == dest)
        {
            count++;

            /* 临时存储一个结果 */
            Result oneResult;
            oneResult.route.clear();
            oneResult.timeCost.day = 0;
            oneResult.timeCost.hour = 0;
            oneResult.timeCost.minute = 0;
            oneResult.moenyCost = 0;

            /* 记录到达当前结点时的最佳时间 */
            MyTime endTime = expectedDepartTime;
            MyTime currentTime;

            /* 产生调试信息：搜索出的一条路径 */
            /* 并产生一个临时的 oneResult */
            tempCityPath.clear();
            tempS = s;
            std::cout << count << ": ";
            while (!tempS.empty()) {
                tempCity = tempS.top();
                tempS.pop();
                tempCityPath.push_back(tempCity);
            }
            std::reverse(tempCityPath.begin(), tempCityPath.end());
            for (vector<string>::iterator it = tempCityPath.begin(); it != tempCityPath.end() - 1; it++) {

                Path onePath;                               // 临时存储一条路径

                /* 处理各种时间 */
                currentTime = endTime;
                currentTime.day = 0;
                MyTime minOnePathEndTime(1000, 1000, 1000);
                MyTime onePathStartTime;
                MyTime onePathUsedTime;
                MyTime onePathEndTime;

                /* 遍历两城市之间的时刻表 */
                bool flag = false;  // 判断是否有找到，如果没有，天数++
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

                            /* 生成 onePath */
                            onePath.number = query.value("Number").toString();
                            onePath.startTime = onePathStartTime;
                            onePath.endTime = minOnePathEndTime;
                            onePath.timeCost = onePathUsedTime;
                            onePath.moneyCost = query.value("Price").toInt();
                            onePath.tool = query.value("Tran").toString();
                        }
                    }
                }

                /* 没有找到，天数++，再遍历一遍时刻表*/
                if (!flag) {
                    endTime.day += 1;
                    endTime.hour = 0;
                    endTime.minute = 0;
                    it--;
                }

                /* 找到了，向 oneResult 中加入此条路径 */
                else {
                    endTime.hour = 0; endTime.minute = 0;
                    endTime = endTime + minOnePathEndTime;
                    onePath.start = *it;
                    onePath.end = *(it + 1);
                    oneResult.moenyCost += onePath.moneyCost;
                    oneResult.timeCost = endTime - expectedDepartTime;
                    oneResult.route.push_back(onePath);
                }
            }

            /* 产生调试信息：这条路径的到达时间（从旅客开始时间算起） */
            for (auto a : tempCityPath)
                cout << a << " ";
            cout << "到达时间: ";
            cout << endTime.day << "天" << endTime.hour << "时" << endTime.minute << "分";
            cout << endl;

            /* 更新最优值 */
            if (endTime < minEndTime) {
                minEndTime = endTime;
                bestPath = tempCityPath;
                result = oneResult;
            }

            /* 回溯，搜索吓一条路径 */
            s.pop();
            visited[findIndex(cityList, dest)][0] = false;
        }

        /* 继续往下搜索 */
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

            /* 搜索到了下一个结点 */
            if (n != -1 && s.size() < 4)
            {
               next = cityList[n];
               s.push(next);
               visited[findIndex(cityList, top)][n + 1] = true;
               visited[n][0] = true;
            }

            /* 如果没有找到下一个结点（此方向遍历结束）或路径过长（不可能时最优解了）*/
            /* 回溯，向另一个方向继续搜索下一条路径 */
            else
            {
               s.pop();
               for (unsigned long i = 0; i < cityNum + 1; i++)
                 visited[findIndex(cityList, top)][i] = false;
            }
        }
    }

    /* 产生调试信息：计算得出的最优值（最小到达时间）*/
    cout << endl;
    cout << "Best: " << minEndTime.day << "天" << minEndTime.hour << "时" << minEndTime.minute << "分" << endl;

    /* 向用户交互界面返回结果信息 */
    log.append(QString("出发地: %1    目的地: %2\n\n").arg(QString::fromStdString(depart)).arg(QString::fromStdString(dest)));
    log.append("旅游线路:\n");
    for (vector<Path>::iterator it = result.route.begin(); it != result.route.end(); it++) {
        log.append(QString("路线: %1--->%2\n").arg(QString::fromStdString(it->start)).arg((QString::fromStdString(it->end))));
        log.append(QString("交通工具: %1    ").arg(it->tool));
        log.append(QString("车次/航班号: %1\n").arg(it->number));
        log.append(QString("花费金额: %1\n").arg(it->moneyCost));
        if (it->endTime.day == 0)
            log.append(QString("出发时间: %1时%2分 到达时间: %3时%4分\n").arg(it->startTime.hour).arg(it->startTime.minute).arg(it->endTime.hour).arg(it->endTime.minute));
        else
            log.append(QString("到达时间: 第%3天%4时%5分\n").arg(it->endTime.day + 1).arg(it->endTime.hour).arg(it->endTime.minute));
        log.append(QString("用时: %1天%2时%3分\n\n").arg(it->timeCost.day).arg(it->timeCost.hour).arg(it->timeCost.minute));
    }
    log.append(QString("总用时: %1天%2时%3分\n").arg(result.timeCost.day).arg(result.timeCost.hour).arg(result.timeCost.minute));
    log.append(QString("总花费金额: %1\n").arg(result.moenyCost));
    destTime = minEndTime;          // 生成到达时间

    /* 释放内存 */
    for (unsigned long i=0; i < cityNum;i++)
        delete[] visited[i];
    delete[] visited;
}

/**
 * @brief Strategy::timeLimitStrategy
 * @param log
 * @author hzy
 *
 * 还没写
 */
void Strategy::timeLimitStrategy(QString &log)
{
    // 用策略一算出一个方案，看时间满不满足预期
    // 用策略二计算出a和b两点的最短时间，如果用户输入的预期到达时间比最短时间还小，直接报错
    // 若用户输入比最短时间大，再遍历a和b两点之间的所有路线，每条路线都按最短时间计算(类似策略二）。选出所有路线中满足用户时间预期的路线，再从中选出花费最少的路线输出

    if(depart==dest)
    {
        log.append(QString("您的出发城市和到达城市一样，请重新选择"));
        return;
    }
    if(expectedDestTime<expectedDepartTime)
    {
        log.append(QString("您的期望到达时间选择有误，请重新选择"));
        return;
    }

    /* 连接数据库 */
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + QString("/travel_query.db"));
    if (!db.open())
        log.append("Failed to connect to database\n");

    /* 配置接口 */
    QSqlQuery query(db);
    query.exec("select * from time_table");

    /* 读取数据库，读入所有要用的城市 */
    vector<string> cityList;
    set<string> _set;
    string str;
    while (query.next()) {
        str = query.value("Dep").toString().toStdString();
        _set.insert(str);
    }
    set<string>::iterator iter;
    for (iter = _set.begin(); iter != _set.end(); iter++) {
        cityList.push_back(*iter);
    }

    /* 建立算法所需要的 Graph 类 */
    Graph G(cityList.size());
    G.setVexsList(cityList);

    /* 产生调试信息：
     * 打印算法所需要的以花费最少为权值的邻接矩阵
     */
    vector<string>::iterator i, j;
    int value;
    QString select;
    for (i = cityList.begin(); i != cityList.end(); i++)
        for (j = cityList.begin(); j != cityList.end(); j++) {
            if (*i == *j);
            else {
                QString s1 = QString::fromStdString(*i);
                QString s2 = QString::fromStdString(*j);
                select = QString("select * from time_table where Dep='%1' and Dest='%2' order by Price asc limit 1").arg(s1).arg(s2);
                query.exec(select);
                query.first();
                value = query.value("Price").toInt();
                G.setValue(*i, *j, value);
                QString MyTimeCost = query.value("Time_Cost").toString();
                MyTime t1;
                t1.day = 0;
                t1.hour = MyTimeCost.section(":", 0, 0).toUShort();
                t1.minute = MyTimeCost.section(":", 1, 1).toUShort();
                G.setTimeTableValue(*i, *j, t1);
            }
        }

     G.shortestPathDJ(depart, dest, result);    // 调用算法

     vector<Path>::iterator iter2;
     MyTime timeUsed;
     unsigned short day = 0;
     /* 按照刚才算法产生的路径重新遍历，得出完整的全部需要的信息 */
     QString start;
     QString end;
     MyTime period, tempPeriod;
     QString codeNumber, methodTool;
     for (iter2 = result.route.begin(); iter2 != result.route.end(); iter2++) {
        start = QString::fromStdString((*iter2).start);
        end = QString::fromStdString((*iter2).end);
        timeUsed = G.getTimeTableValue((*iter2).start, (*iter2).end);
        select = QString("select * from time_table where Dep='%1' and Dest='%2' and Price=%3 order by Time_Cost").arg(start).arg(end).arg((*iter2).moneyCost);
        query.exec(select);
        query.first();
        period.parseString(query.value("Dep_Time").toString());
        codeNumber = query.value("Number").toString();
        methodTool = query.value("Tran").toString();
        while(query.next()) {
            tempPeriod.parseString(query.value("Dep_Time").toString());
            if(tempPeriod < period) {
                period = tempPeriod;
                codeNumber = query.value("Number").toString();
                methodTool = query.value("Tran").toString();
            }
        }
        if(start == QString::fromStdString(depart)) {
            MyTime earliest = period;
            query.first();
            while(departTime > period) {
                tempPeriod.parseString(query.value("Dep_Time").toString());
                if(tempPeriod > period)
                    period = tempPeriod;
                if(!query.next())
                    break;
            }
            if(departTime > period) {
                departTime = earliest;
                period = earliest;
                departTime.day += 1;
            } else {
                departTime = period;
            }
            destTime = departTime + timeUsed;
            day += destTime.day;
        } else {
            query.first();
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
     }
     destTime.day = day;
     timeUsed = destTime - expectedDepartTime;

     if(destTime<expectedDestTime)
     {

         //----------------------------->>输出策略一的结果
         log.append(QString("出发地: %1    目的地: %2\n\n").arg(QString::fromStdString(depart)).arg(QString::fromStdString(dest)));
         log.append("旅游线路:\n");
         log.append(QString("路线: %1--->%2\n").arg(start).arg(end));
         log.append(QString("交通工具: %1    ").arg(methodTool));
         log.append(QString("车次/航班号: %1\n").arg(codeNumber));
         log.append(QString("花费金额: %1\n").arg((*iter2).moneyCost));
         log.append(QString("出发时间: %1时%2分    ").arg(period.hour).arg(period.minute));
         log.append(QString("用时: %1天%2时%3分\n\n").arg(timeUsed.day).arg(timeUsed.hour).arg(timeUsed.minute));
         log.append(QString("区间用时: %1天%2时%3分\n").arg(timeUsed.day).arg(timeUsed.hour).arg(timeUsed.minute));
         log.append(QString("总花费金额: %1\n").arg(result.moenyCost));
     }
     else
     {
 //-----------------------------------------------------
         //运行策略二
         stack<string> s;                // 策略二算法运行时记录路径的栈
         string top;                     // 栈顶元素
         long n;                         // 临时存储序号
         string next;                    // 需要访问的下一个元素
         int count = 0;                  // 记录找到的所有路径的个数
         string tempCity;                // 临时存储元素
         stack<string> tempS;            // 临时存储栈，便于提取栈中元素且不破坏原栈内容
         vector<string> tempCityPath;    // 临时存储找到的路径
         vector<string> bestPath;        // 存储找到的最佳路径
         MyTime minEndTime(10000, 10000, 10000); // 存储迭代过程中的最小到达时间
         unsigned long cityNum = cityList.size();// 存储城市个数
         bool **visited = new bool*[cityNum];    // 访问辅助数组，visited[i][j] 表示：
                                                 // j = 0: i 是否被访问过
                                                 // j != 0 : 访问 i 之后 j 是否被访问郭
         /* 为访问辅助矩阵分配内存 */
         for(unsigned long i = 0; i < cityNum; i++)
             visited[i] = new bool[cityNum + 1];
         /* 初始化访问辅助矩阵 */
         for (unsigned long i = 0; i < cityNum; i++)
             for (unsigned long j = 0; j < cityNum + 1; j++)
                 visited[i][j] = false;

         /* 提高算法的运算效率，将终点放到前面来，以便先行入栈 */
         long destIndex = findIndex(cityList, dest);
         string temp = cityList[0];
         cityList[0] = dest;
         cityList[destIndex] = temp;

         /* 初始化 result */
         result.route.clear();
         result.timeCost.day = 0;
         result.timeCost.hour = 0;
         result.timeCost.minute = 0;

         /* 算法运算部分 */
         s.push(depart);                                 // 出发地先行入栈
         visited[findIndex(cityList, depart)][0] = true; // 已访问出发地
         /* 搜索所有路径，并根据进行情况回溯 */
         while(!s.empty())
         {
             top = s.top();

             /* 找到了一条从起点到终点的路径 */
             if (top == dest)
             {
                 count++;

                 /* 临时存储一个结果 */
                 Result oneResult;
                 oneResult.route.clear();
                 oneResult.timeCost.day = 0;
                 oneResult.timeCost.hour = 0;
                 oneResult.timeCost.minute = 0;
                 oneResult.moenyCost = 0;

                 /* 记录到达当前结点时的最佳时间 */
                 MyTime endTime = expectedDepartTime;
                 MyTime currentTime;

                 /* 产生调试信息：搜索出的一条路径 */
                 /* 并产生一个临时的 oneResult */
                 tempCityPath.clear();
                 tempS = s;
                 std::cout << count << ": ";
                 while (!tempS.empty()) {
                     tempCity = tempS.top();
                     tempS.pop();
                     tempCityPath.push_back(tempCity);
                 }
                 std::reverse(tempCityPath.begin(), tempCityPath.end());
                 for (vector<string>::iterator it = tempCityPath.begin(); it != tempCityPath.end() - 1; it++) {

                     Path onePath;                               // 临时存储一条路径

                     /* 处理各种时间 */
                     currentTime = endTime;
                     currentTime.day = 0;
                     MyTime minOnePathEndTime(1000, 1000, 1000);
                     MyTime onePathStartTime;
                     MyTime onePathUsedTime;
                     MyTime onePathEndTime;

                     /* 遍历两城市之间的时刻表 */
                     bool flag = false;  // 判断是否有找到，如果没有，天数++
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

                                 /* 生成 onePath */
                                 onePath.number = query.value("Number").toString();
                                 onePath.startTime = onePathStartTime;
                                 onePath.endTime = minOnePathEndTime;
                                 onePath.timeCost = onePathUsedTime;
                                 onePath.moneyCost = query.value("Price").toInt();
                                 onePath.tool = query.value("Tran").toString();
                             }
                         }
                     }

                     /* 没有找到，天数++，再遍历一遍时刻表*/
                     if (!flag) {
                         endTime.day += 1;
                         endTime.hour = 0;
                         endTime.minute = 0;
                         it--;
                     }

                     /* 找到了，向 oneResult 中加入此条路径 */
                     else {
                         endTime.hour = 0; endTime.minute = 0;
                         endTime = endTime + minOnePathEndTime;
                         onePath.start = *it;
                         onePath.end = *(it + 1);
                         oneResult.moenyCost += onePath.moneyCost;
                         oneResult.timeCost = endTime - expectedDepartTime;
                         oneResult.route.push_back(onePath);
                     }
                 }

                 /* 产生调试信息：这条路径的到达时间（从旅客开始时间算起） */
                 for (auto a : tempCityPath)
                     cout << a << " ";
                 cout << "到达时间: ";
                 cout << endTime.day << "天" << endTime.hour << "时" << endTime.minute << "分";
                 cout << endl;

                 /* 更新最优值 */
                 if (endTime < minEndTime) {
                     minEndTime = endTime;
                     bestPath = tempCityPath;
                     result = oneResult;
                 }
                 /* 回溯，搜索吓一条路径 */
                 s.pop();
                 visited[findIndex(cityList, dest)][0] = false;
             }

             /* 继续往下搜索 */
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

                 /* 搜索到了下一个结点 */
                 if (n != -1 && s.size() < 4)
                 {
                    next = cityList[n];
                    s.push(next);
                    visited[findIndex(cityList, top)][n + 1] = true;
                    visited[n][0] = true;
                 }

                 /* 如果没有找到下一个结点（此方向遍历结束）或路径过长（不可能时最优解了）*/
                 /* 回溯，向另一个方向继续搜索下一条路径 */
                 else
                 {
                    s.pop();
                    for (unsigned long i = 0; i < cityNum + 1; i++)
                      visited[findIndex(cityList, top)][i] = false;
                 }
             }
         }
         destTime = minEndTime;          // 生成到达时
         /* 释放内存 */
         for (unsigned long i=0; i < cityNum;i++)
             delete[] visited[i];
         delete[] visited;

         if(destTime>expectedDestTime)
         {
             result.moenyCost=0;
             log.append(QString("你期望的到达时间太早，系统无法匹配路线！"));
         }
         else
         {
 //-----------------------------再遍历a和b两点之间的所有路线，每条路线都按最短时间计算
             vector<Result> timeLimitResults;   //所有满足时间条件的result
             stack<string> s;                // 策略二算法运行时记录路径的栈
             string top;                     // 栈顶元素
             long n;                         // 临时存储序号
             string next;                    // 需要访问的下一个元素
             int count = 0;                  // 记录找到的所有路径的个数
             string tempCity;                // 临时存储元素
             stack<string> tempS;            // 临时存储栈，便于提取栈中元素且不破坏原栈内容
             vector<string> tempCityPath;    // 临时存储找到的路径
             vector<string> bestPath;        // 存储找到的最佳路径
            // MyTime minEndTime(10000, 10000, 10000); // 存储迭代过程中的最小到达时间
             unsigned long cityNum = cityList.size();// 存储城市个数
             bool **visited = new bool*[cityNum];    // 访问辅助数组，visited[i][j] 表示：
                                                     // j = 0: i 是否被访问过
                                                     // j != 0 : 访问 i 之后 j 是否被访问郭
             /* 为访问辅助矩阵分配内存 */
             for(unsigned long i = 0; i < cityNum; i++)
                 visited[i] = new bool[cityNum + 1];
             /* 初始化访问辅助矩阵 */
             for (unsigned long i = 0; i < cityNum; i++)
                 for (unsigned long j = 0; j < cityNum + 1; j++)
                     visited[i][j] = false;

             /* 提高算法的运算效率，将终点放到前面来，以便先行入栈 */
             long destIndex = findIndex(cityList, dest);
             string temp = cityList[0];
             cityList[0] = dest;
             cityList[destIndex] = temp;

             /* 初始化 result */
             result.route.clear();
             result.timeCost.day = 0;
             result.timeCost.hour = 0;
             result.timeCost.minute = 0;

             /* 算法运算部分 */
             s.push(depart);                                 // 出发地先行入栈
             visited[findIndex(cityList, depart)][0] = true; // 已访问出发地
             /* 搜索所有路径，并根据进行情况回溯 */
             while(!s.empty())
             {
                 top = s.top();

                 /* 找到了一条从起点到终点的路径 */
                 if (top == dest)
                 {
                     count++;

                     /* 临时存储一个结果 */
                     Result oneResult;
                     oneResult.route.clear();
                     oneResult.timeCost.day = 0;
                     oneResult.timeCost.hour = 0;
                     oneResult.timeCost.minute = 0;
                     oneResult.moenyCost = 0;

                     /* 记录到达当前结点时的最佳时间 */
                     MyTime endTime = expectedDepartTime;
                     MyTime currentTime;

                     /* 产生调试信息：搜索出的一条路径 */
                     /* 并产生一个临时的 oneResult */
                     tempCityPath.clear();
                     tempS = s;
                     std::cout << count << ": ";
                     while (!tempS.empty()) {
                         tempCity = tempS.top();
                         tempS.pop();
                         tempCityPath.push_back(tempCity);
                     }
                     std::reverse(tempCityPath.begin(), tempCityPath.end());
                     for (vector<string>::iterator it = tempCityPath.begin(); it != tempCityPath.end() - 1; it++) {

                         Path onePath;                               // 临时存储一条路径

                         /* 处理各种时间 */
                         currentTime = endTime;
                         currentTime.day = 0;
                         MyTime minOnePathEndTime(1000, 1000, 1000);
                         MyTime onePathStartTime;
                         MyTime onePathUsedTime;
                         MyTime onePathEndTime;

                         /* 遍历两城市之间的时刻表 */
                         bool flag = false;  // 判断是否有找到，如果没有，天数++
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

                                     /* 生成 onePath */
                                     onePath.number = query.value("Number").toString();
                                     onePath.startTime = onePathStartTime;
                                     onePath.endTime = minOnePathEndTime;
                                     onePath.timeCost = onePathUsedTime;
                                     onePath.moneyCost = query.value("Price").toInt();
                                     onePath.tool = query.value("Tran").toString();
                                 }
                             }
                         }

                         /* 没有找到，天数++，再遍历一遍时刻表*/
                         if (!flag) {
                             endTime.day += 1;
                             endTime.hour = 0;
                             endTime.minute = 0;
                             it--;
                         }

                         /* 找到了，向 oneResult 中加入此条路径 */
                         else {
                             endTime.hour = 0; endTime.minute = 0;
                             endTime = endTime + minOnePathEndTime;
                             onePath.start = *it;
                             onePath.end = *(it + 1);
                             oneResult.moenyCost += onePath.moneyCost;
                             oneResult.timeCost = endTime - expectedDepartTime;
                             oneResult.route.push_back(onePath);
                         }
                     }

                     //将满足时间要求的results加入timeLimitResults
                     if(endTime<expectedDestTime)
                     {
                         timeLimitResults.push_back(oneResult);
                     }

                     /* 回溯，搜索下一条路径 */
                     s.pop();
                     visited[findIndex(cityList, dest)][0] = false;
                 }

                 /* 继续往下搜索 */
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

                     /* 搜索到了下一个结点 */
                     if (n != -1 && s.size() < 4)
                     {
                        next = cityList[n];
                        s.push(next);
                        visited[findIndex(cityList, top)][n + 1] = true;
                        visited[n][0] = true;
                     }

                     /* 如果没有找到下一个结点（此方向遍历结束）或路径过长（不可能时最优解了）*/
                     /* 回溯，向另一个方向继续搜索下一条路径 */
                     else
                     {
                        s.pop();
                        for (unsigned long i = 0; i < cityNum + 1; i++)
                          visited[findIndex(cityList, top)][i] = false;
                     }
                 }
             }

             /* 释放内存 */
             for (unsigned long i=0; i < cityNum;i++)
                 delete[] visited[i];
             delete[] visited;


             //选出花费最少的result
             result.moenyCost=1000000;
             vector<Result>::iterator iter;
             for(iter=timeLimitResults.begin();iter!=timeLimitResults.end();iter++)
             {
                 if((*iter).moenyCost<result.moenyCost)
                     result=(*iter);
             }

             /* 向用户交互界面返回结果信息 */
             log.append(QString("出发地: %1    目的地: %2\n\n").arg(QString::fromStdString(depart)).arg(QString::fromStdString(dest)));
             log.append("旅游线路:\n");
             for (vector<Path>::iterator it = result.route.begin(); it != result.route.end(); it++) {
                 log.append(QString("路线: %1--->%2\n").arg(QString::fromStdString(it->start)).arg((QString::fromStdString(it->end))));
                 log.append(QString("交通工具: %1    ").arg(it->tool));
                 log.append(QString("车次/航班号: %1\n").arg(it->number));
                 log.append(QString("花费金额: %1\n").arg(it->moneyCost));
                 if (it->endTime.day == 0)
                     log.append(QString("出发时间: %1时%2分 到达时间: %3时%4分\n").arg(it->startTime.hour).arg(it->startTime.minute).arg(it->endTime.hour).arg(it->endTime.minute));
                 else
                     log.append(QString("到达时间: 第%3天%4时%5分\n").arg(it->endTime.day + 1).arg(it->endTime.hour).arg(it->endTime.minute));
                 log.append(QString("用时: %1天%2时%3分\n\n").arg(it->timeCost.day).arg(it->timeCost.hour).arg(it->timeCost.minute));
             }
             log.append(QString("总用时: %1天%2时%3分\n").arg(result.timeCost.day).arg(result.timeCost.hour).arg(result.timeCost.minute));
             log.append(QString("总花费金额: %1\n").arg(result.moenyCost));
         }
     }
     /* 关闭数据库 */
     db.close();
}
