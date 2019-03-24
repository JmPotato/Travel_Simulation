#ifndef TOURIST_H
#define TOURIST_H

#include "MyTime.h"
#include "Strategy.h"

#include <string>

using namespace std;

class Tourist {
    unsigned short id;      // 乘客编号
    string depart;          // 出发地
    string dest;            // 目的地
    int type;               // 旅客选择的策略：
                            // 1:策略一，2:策略二，3：策略三
    MyTime departTime;        // 期望出发时间
    MyTime destTime;          // 期望到达时间
    string location;        // 旅客当前位置
public:
    QString log;
    Tourist(string start, string end, int strategy);
    Tourist(string start, string end, int strategy, MyTime t);
    unsigned short getId();
    string getDepart();
    string getDest();
    int getType();
    string getLocation();
    void getStrategy();
};

#endif // TOURIST_H
