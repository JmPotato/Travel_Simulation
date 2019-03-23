#ifndef STRATEGY_H
#define STRATEGY_H

#include <string>
#include "Result.h"
#include "MyTime.h"

using std::string;

class Strategy
{
public:
    Strategy(int t, string d1, string d2, MyTime departT, MyTime destT);

    void startStrategy();
private:
    int type;
    string depart;
    string dest;
    MyTime departTime;
    MyTime destTime;

    Result result;

    // 三种策略
    void cheapestStrategy();
    void fastestStrategy();
    void timeLimitStrategy();

};

#endif // STRATEGY_H
