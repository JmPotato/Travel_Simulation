#ifndef STRATEGY_H
#define STRATEGY_H

#include <string>
#include <QSqlDatabase>
#include "Result.h"
#include "MyTime.h"

using std::string;

class Strategy
{
public:
    /* 构造函数 */
    Strategy(int t, string d1, string d2, MyTime expectedDepartT, MyTime expectedDestT);
    ~Strategy();

    /* 策略开始的驱动函数 */
    void startStrategy(QString &log);

    /* 需要与其他类交互的数据结构 */
    MyTime departTime;              // 出发时间
    MyTime destTime;                // 到达时间
    MyTime expectedDepartTime;      // 期望的出发时间
    MyTime expectedDestTime;        // 期望的到达时间
    /* 策略产生的结果 */
    Result result;  // 不一定会被使用
private:
    QSqlDatabase db;
    /* 出发地与目的地 */
    string depart;
    string dest;

    /* 策略类型 */
    int type;

    /* 三种策略分别对应的算法函数 */
    Result cheapestStrategy(QString &log, string d1, string d2, MyTime expectedDepartT);
    Result fastestStrategy(QString &log, string d1, string d2, MyTime expectedDepartT);
    Result timeLimitStrategy(QString &log);
};

#endif // STRATEGY_H
