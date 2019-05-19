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
    /* 连接数据库 */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + QString("/travel_query.db"));
    if (!db.open())
        cout << "Failed to connect to database";
}

Strategy::Strategy(int t, std::string d1, std::string d2, QStringList cities, vector<int> hours, MyTime expectedDepartT, MyTime expectedDestT)
{
    type = t;
    depart = d1;
    dest = d2;
    passCities = cities;
    passHours = hours;
    departTime = expectedDepartT;
    expectedDepartTime = expectedDepartT;
    expectedDestTime = expectedDestT;
    result.timeCost.day = 0;
    result.timeCost.hour = 0;
    result.timeCost.minute = 0;
    result.moenyCost = 0;
    /* 连接数据库 */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + QString("/travel_query.db"));
    if (!db.open())
        cout << "Failed to connect to database";
}

Strategy::~Strategy()
{
    /* 关闭数据库 */
    db.close();
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
            cheapestStrategy(log, depart, dest, expectedDepartTime);
            break;
        case 2:
            fastestStrategy(log, depart, dest, expectedDepartTime);
            break;
        case 3:
            timeLimitStrategy(log);
            break;
    }
}

void Strategy::startPassStrategy(QString &log)
{
    switch (type) {
        case 1:
            cheapestPassStrategy(log);
            break;
        case 2:
            fastestPassStrategy(log);
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
Result Strategy::cheapestStrategy(QString &log, string d1, string d2, MyTime expectedDepartT)
{
    //cout << "start ";
    //expectedDepartT.print();
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
    // G.printMatrix();
    /* ----------------------------------------------------------------------------- */
     result.route.clear();
     G.shortestPathDJ(d1, d2, result);    // 调用算法

     /* 产生交互界面所需要的结果信息 */
     //log.append(QString("出发地: %1    目的地: %2\n\n").arg(QString::fromStdString(d1)).arg(QString::fromStdString(d2)));
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
        if(start == QString::fromStdString(d1)) {
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

                departTime.hour=period.hour;
                departTime.minute=period.minute;
            }
            period = departTime;
            period.day = departTime.day + expectedDepartT.day;
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
            period.day = day + expectedDepartT.day;
            day += destTime.day;
        }
        iter2->tool = methodTool;
        iter2->number = codeNumber;
        iter2->startTime = period;
        iter2->endTime = period + timeUsed;
        iter2->timeCost = timeUsed;
        log.append(QString("路线: %1--->%2\n").arg(start).arg(end));
        log.append(QString("交通工具: %1    ").arg(methodTool));
        log.append(QString("车次/航班号: %1\n").arg(codeNumber));
        log.append(QString("花费金额: %1\n").arg((*iter2).moneyCost));
        log.append(QString("出发时间: %1时%2分    ").arg(period.hour).arg(period.minute));
        log.append(QString("用时: %1天%2时%3分\n\n").arg(timeUsed.day).arg(timeUsed.hour).arg(timeUsed.minute));
     }
     destTime.day = day + expectedDepartT.day;
     timeUsed = destTime - expectedDepartT;
     result.timeCost = timeUsed;
     log.append(QString("区间用时: %1天%2时%3分\n").arg(timeUsed.day).arg(timeUsed.hour).arg(timeUsed.minute));
     log.append(QString("总花费金额: %1\n").arg(result.moenyCost));
     log.append(QString("---------------------------------------\n"));
     // 输出结束
     //result.timeCost.print();
     result.destTime = destTime;
     //cout << "end ";
     // destTime.print();
     return result;
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
 * @brief incOneNumber
 * @param oneNumber
 * @param radix
 * @author hzy
 */
void incOneNumber(vector<int> &oneNumber, int radix)
{

    oneNumber[oneNumber.size() - 1]++;
    for (unsigned long i = oneNumber.size() - 1; i > 0; i--) {
        if (oneNumber[i] >= radix) {
            oneNumber[i] = 0;
            oneNumber[i - 1]++;
        }
    }
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
Result Strategy::fastestStrategy(QString &log, string d1, string d2, MyTime expectedDepartT)
{

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
    long destIndex = findIndex(cityList, d2);
    string temp = cityList[0];
    cityList[0] = d2;
    cityList[destIndex] = temp;

    /* 初始化 result */
    result.route.clear();
    result.timeCost.day = 0;
    result.timeCost.hour = 0;
    result.timeCost.minute = 0;

    /* 算法运算部分 */
    s.push(d1);                                 // 出发地先行入栈
    visited[findIndex(cityList, d1)][0] = true; // 已访问出发地
    /* 搜索所有路径，并根据进行情况回溯 */
    while(!s.empty())
    {
        top = s.top();

        /* 找到了一条从起点到终点的路径 */
        if (top == d2)
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
            MyTime endTime = expectedDepartT;
            MyTime currentTime;

            /* 产生调试信息：搜索出的一条路径 */
            /* 并产生一个临时的 oneResult */
            tempCityPath.clear();
            tempS = s;
            //std::cout << count << ": ";
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
                        //onePathStartTime.day = endTime.day;
                        onePathEndTime = onePathStartTime + onePathUsedTime;
                        if (onePathEndTime < minOnePathEndTime) {
                            minOnePathEndTime = onePathEndTime;

                            /* 生成 onePath */
                            onePath.number = query.value("Number").toString();
                            onePath.startTime = onePathStartTime;
                            onePath.startTime.day = endTime.day;
                            onePath.endTime = onePath.startTime + onePathUsedTime;
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
                    oneResult.timeCost = endTime - expectedDepartT;
                    oneResult.route.push_back(onePath);
                }
            }

            /* 产生调试信息：这条路径的到达时间（从旅客开始时间算起） */
//            for (auto a : tempCityPath)
//                cout << a << " ";
//            cout << "到达时间: ";
//            cout << endTime.day << "天" << endTime.hour << "时" << endTime.minute << "分";
//            cout << endl;

            /* 更新最优值 */
            if (endTime < minEndTime) {
                minEndTime = endTime;
                bestPath = tempCityPath;
                result = oneResult;
            }

            /* 回溯，搜索吓一条路径 */
            s.pop();
            visited[findIndex(cityList, d2)][0] = false;
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
    //cout << endl;
    //cout << "Best: " << minEndTime.day << "天" << minEndTime.hour << "时" << minEndTime.minute << "分" << endl;

    /* 向用户交互界面返回结果信息 */
    log.append(QString("出发地: %1    目的地: %2\n\n").arg(QString::fromStdString(d1)).arg(QString::fromStdString(d2)));
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
    log.append(QString("---------------------------------------\n"));
    destTime = minEndTime;          // 生成到达时间

    /* 释放内存 */
    for (unsigned long i=0; i < cityNum;i++)
        delete[] visited[i];
    delete[] visited;
    result.destTime = destTime;
    return result;
}



void add_result(Result &main, Result add)
{
    main.timeCost = main.timeCost + (add.destTime - main.destTime);
    main.destTime = add.destTime;
    main.moenyCost += add.moenyCost;
    for (auto path : add.route)
        main.route.push_back(path);
}

Result Strategy::cheapestPassStrategy(QString &log)
{
    //int totalMoneyCost = 0;

    int cityNumber = passCities.length();
    if(cityNumber == 0)
    {
        Result aResult = cheapestStrategy(log,depart,dest,expectedDepartTime);
        //totalMoneyCost += aResult.moenyCost;
        //aResult.moenyCost = totalMoneyCost;
        //result.moenyCost = totalMoneyCost;
        return aResult;
    }
    else if(cityNumber > 0 )
    {
        Result firstResult;
        firstResult = cheapestStrategy(log,depart,passCities.at(0).toStdString(),expectedDepartTime);
        //totalMoneyCost += firstResult.moenyCost;
        MyTime newDepartTime(0,passHours[0],0);
        newDepartTime = newDepartTime + firstResult.destTime;
        //cout << "new ";
       // newDepartTime.print();
        int i=0;
        if(cityNumber > 1)
        {
            for(; i<cityNumber-1; i++)
            {
                Result middleResult = cheapestStrategy(log,passCities.at(i).toStdString(),passCities.at(i+1).toStdString(),newDepartTime);
                add_result(firstResult, middleResult);
                //totalMoneyCost += middleResult.moenyCost;
                MyTime passTime(0,passHours[i+1],0);
                newDepartTime = middleResult.destTime + passTime;
                cout << "new ";
                //newDepartTime.print();
            }
        }
        Result lastResult = cheapestStrategy(log,passCities.at(i).toStdString(),dest,newDepartTime);
        add_result(firstResult, lastResult);
        //totalMoneyCost += lastResult.moenyCost;
        //lastResult.moenyCost = totalMoneyCost;
        //result.moenyCost = totalMoneyCost;
        result = firstResult;
        return firstResult;
    }
}

Result Strategy::fastestPassStrategy(QString &log)
{
   // int totalMoneyCost = 0;
    int cityNumber = passCities.length();
    if(cityNumber == 0)
    {
        Result aResult = fastestStrategy(log,depart,dest,expectedDepartTime);
        //totalMoneyCost += aResult.moenyCost;
        //result.moenyCost = totalMoneyCost;
        //aResult.moenyCost = totalMoneyCost;
        return aResult;
    }
    else if(cityNumber > 0 )
    {
        Result firstResult;
        firstResult = fastestStrategy(log,depart,passCities.at(0).toStdString(),expectedDepartTime);
        //totalMoneyCost += firstResult.moenyCost;
        MyTime newDepartTime(0,passHours[0],0);
        newDepartTime = newDepartTime + firstResult.destTime;
        int i=0;
        if(cityNumber > 1)
        {
            for(; i<cityNumber-1; i++)
            {
                Result middleResult = fastestStrategy(log,passCities.at(i).toStdString(),passCities.at(i+1).toStdString(),newDepartTime);
                add_result(firstResult, middleResult);
                //totalMoneyCost += middleResult.moenyCost;
                MyTime passTime(0,passHours[i+1],0);
                newDepartTime = middleResult.destTime + passTime;
            }
        }
        Result lastResult = fastestStrategy(log,passCities.at(i).toStdString(),dest,newDepartTime);
        add_result(firstResult, lastResult);
        //totalMoneyCost += lastResult.moenyCost;
        //result.moenyCost = totalMoneyCost;
        //lastResult.moenyCost = totalMoneyCost;
        result = firstResult;
        return firstResult;
    }
}

Result Strategy::timeLimitStrategy(QString &log)
{
    // 用策略一算出一个方案，看时间满不满足预期
    // 用策略二计算出a和b两点的最短时间，如果用户输入的预期到达时间比最短时间还小，直接报错
    // 若用户输入比最短时间大，再遍历a和b两点之间的所有路线，每条路线都按最短时间计算(类似策略二）。选出所有路线中满足用户时间预期的路线，再从中选出花费最少的路线输出
    if(expectedDestTime == expectedDepartTime)
    {
        log.append(QString("您的期望到达时间选择有误，请重新选择"));
        return result;
    }

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

    log.append(QString("出发地: %1    目的地: %2\n\n").arg(QString::fromStdString(depart)).arg(QString::fromStdString(dest)));
    Result aResult;
    QString tempString;
    //aResult = cheapestStrategy(tempString, dest, depart, expectedDepartTime);
    aResult = cheapestPassStrategy(log);
    //aResult.timeCost.print();
    if(aResult.timeCost + expectedDepartTime < expectedDestTime)
    {
        result = aResult;
    }
    else {
 //-----------------------------------------------------
        //运行策略二
        //aResult = fastestStrategy(tempString, depart, dest, expectedDepartTime);
        aResult = fastestPassStrategy(log);
        log.clear();
        //aResult.timeCost.print();
        //expectedDestTime.print();
        //expectedDepartTime.print();
        if(expectedDestTime < aResult.destTime) {
             //cout << "!!!!" << endl;
             result.moenyCost = 0;
             log.append(QString("你期望的到达时间太早，系统无法匹配路线！"));
        }
        else /*if (passCities.length() == 0)*/{
            stack<string> s;                // 策略二算法运行时记录路径的栈
            string top;                     // 栈顶元素
            long n;                         // 临时存储序号
            string next;                    // 需要访问的下一个元素
            //int count = 0;                  // 记录找到的所有路径的个数
            string tempCity;                // 临时存储元素
            stack<string> tempS;            // 临时存储栈，便于提取栈中元素且不破坏原栈内容
            vector<string> tempCityPath;    // 临时存储找到的路径
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



            for (unsigned long i = 0; i < cityNum; i++)
                for (unsigned long j = 0; j < cityNum + 1; j++)
                    visited[i][j] = false;
            result.moenyCost = MaxInt;
            while (!s.empty()) {
                s.pop();
            }

            s.push(depart);
            visited[findIndex(cityList, depart)][0] = true;
            int m = 0;
            bool allPassCitiesFound = false;
            int c = 0;
            /* 搜索所有路径，并根据进行情况回溯 */
            while(!s.empty())
            {
                //c++;
                top = s.top();

                if (m < passCities.length() && top == passCities.at(m).toStdString()) {
                    m++;
                }

                if (m == passCities.length())
                    allPassCitiesFound = true;
                else
                    allPassCitiesFound = false;
                /* 找到了一条从起点到终点的路径 */
                if (top == dest)
                {
                    c++;
                    if (allPassCitiesFound) {
                        MyTime endTime = expectedDepartTime;
                        MyTime currentTime = endTime;
                        currentTime.day = 0;
                        currentTime.hour = 0;
                        currentTime.minute = 0;

                        /* 产生调试信息：搜索出的一条路径 */
                        /* 并产生一个临时的 oneResult */
                        tempCityPath.clear();
                        tempS = s;

                        while (!tempS.empty()) {
                            tempCity = tempS.top();
                            tempS.pop();
                            tempCityPath.push_back(tempCity);
                        }
                        std::reverse(tempCityPath.begin(), tempCityPath.end());
                        /* 建立某条路径的所有交通方式组合矩阵 */
                        vector<vector<Path>> allMethodForOneRoute;
                        unsigned long maxLen = 0;
                        for (unsigned long it = 0; it < tempCityPath.size() - 1; it++) {
                            Path onePath;
                            vector<Path> tempPathVector;
                            onePath.start = tempCityPath[it];
                            onePath.end = tempCityPath[it + 1];
                            query.exec(QString("select * from time_table where Dep='%1' and Dest='%2'").arg(QString::fromStdString(tempCityPath[it])).arg(QString::fromStdString(tempCityPath[it + 1])));
                            while (query.next()) {
                                onePath.tool = query.value("Tran").toString();
                                onePath.number = query.value("Number").toString();
                                onePath.startTime = MyTime(query.value("Dep_Time").toString());
                                onePath.timeCost = MyTime(query.value("Time_Cost").toString());
                                onePath.endTime = onePath.startTime + onePath.timeCost;

                                onePath.moneyCost = query.value("Price").toInt();
                                //allMethodForOneRoute[it].push_back(onePath);
                                tempPathVector.push_back(onePath);
                            }
                            if (tempPathVector.size() > maxLen)
                                maxLen = tempPathVector.size();
                            allMethodForOneRoute.push_back(tempPathVector);
                        }

                        int radix = maxLen;
                        vector<int> oneNumber(allMethodForOneRoute.size(), 0);
                        while (oneNumber[0] != radix) {
                            bool noBetterResult = false;
                            /* 临时存储一个结果 */
                            int q = 0;
                            Result oneResult;
                            oneResult.route.clear();
                            oneResult.timeCost.day = 0;
                            oneResult.timeCost.hour = 0;
                            oneResult.timeCost.minute = 0;
                            oneResult.moenyCost = 0;
                            endTime = expectedDepartTime;
                            currentTime = endTime;
                            for (unsigned long index = 0; index < oneNumber.size(); index++) {
                                if (allMethodForOneRoute[index].size() - 1 < oneNumber[index]) {
                                    oneResult.moenyCost = MaxInt;
                                    break;
                                }
                                else {
                                    int p = oneNumber[index];

                                    if (passCities.contains(QString::fromStdString(allMethodForOneRoute[index][p].start))) {
                                        MyTime aPassTime(0, passHours[q], 0);
                                        q++;
                                        currentTime = currentTime + aPassTime;
                                    }
                                    if (currentTime > allMethodForOneRoute[index][p].startTime)
                                        endTime.day++;
                                    endTime.minute = 0;
                                    endTime.hour = 0;
                                    endTime = endTime + allMethodForOneRoute[index][p].endTime;
                                    if (endTime > expectedDestTime) {
                                        oneResult.moenyCost = MaxInt;
                                        break;
                                    }
                                    currentTime = endTime;
                                    currentTime.day = 0;
                                    Path tempPath = allMethodForOneRoute[index][p];
                                    tempPath.endTime.day = endTime.day;
                                    tempPath.startTime = tempPath.endTime - tempPath.timeCost;
                                    oneResult.route.push_back(tempPath);
                                    oneResult.moenyCost += allMethodForOneRoute[index][p].moneyCost;
                                }
                                oneResult.timeCost = endTime - expectedDepartTime;
                            }

                            if (oneResult.moenyCost < result.moenyCost && !(endTime > expectedDestTime)) {
                                result = oneResult;
                                for (auto path : oneResult.route)
                                    cout << path.start << "->" << path.end << " ";
                                cout << result.timeCost.day << "天" << result.timeCost.hour << "时" << result.timeCost.minute << "分   ";
                                cout << result.moenyCost << "元";
                                cout << "   End Time:" << endTime.day << " " << endTime.hour << " " << endTime.minute << endl;

                            }

                            incOneNumber(oneNumber, radix);
                        }
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
                    if (n != -1 && /*s.size() <= 3 * (m + 1)*/s.size() <= m + 2)
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
                       if (m > 0 && top == passCities.at(m - 1).toStdString())
                           m--;
                       for (unsigned long i = 0; i < cityNum + 1; i++)
                         visited[findIndex(cityList, top)][i] = false;
                    }
                }
            }
            destTime = expectedDepartTime + result.timeCost;          // 生成到达时
            /* 向用户交互界面返回结果信息 */
            log.append("旅游线路:\n");
            for (vector<Path>::iterator it = result.route.begin(); it != result.route.end(); it++) {
                log.append(QString("路线: %1--->%2\n").arg(QString::fromStdString(it->start)).arg((QString::fromStdString(it->end))));
                log.append(QString("交通工具: %1    ").arg(it->tool));
                log.append(QString("车次/航班号: %1\n").arg(it->number));
                log.append(QString("花费金额: %1\n").arg(it->moneyCost));                    log.append(QString("出发时间: %1时%2分 到达时间: %3时%4分\n").arg(it->startTime.hour).arg(it->startTime.minute).arg(it->endTime.hour).arg(it->endTime.minute));
                log.append(QString("用时: %1天%2时%3分\n\n").arg(it->timeCost.day).arg(it->timeCost.hour).arg(it->timeCost.minute));
            }
            log.append(QString("总用时: %1天%2时%3分\n").arg(result.timeCost.day).arg(result.timeCost.hour).arg(result.timeCost.minute));
            log.append(QString("总花费金额: %1\n").arg(result.moenyCost));

            /* 释放内存 */
            for (unsigned long i=0; i < cityNum;i++)
                delete[] visited[i];
            delete[] visited;
                cout << c << endl;
        }

    }

    return result;
}

