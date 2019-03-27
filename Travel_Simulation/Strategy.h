#ifndef STRATEGY_H
#define STRATEGY_H

#include <string>
#include "Result.h"
#include "MyTime.h"

using std::string;

class Strategy
{
public:
    Strategy(int t, string d1, string d2, MyTime expectedDepartT, MyTime expectedDestT);
    void startStrategy(QString &log);
    MyTime departTime;
    MyTime destTime;
    MyTime expectedDepartTime;
    MyTime expectedDestTime;
    Result result;
private:

    string depart;
    string dest;


    int type;


    // 三种策略
    void cheapestStrategy(QString &log);
    void fastestStrategy(QString &log);
    void timeLimitStrategy(QString &log);

};

#endif // STRATEGY_H
