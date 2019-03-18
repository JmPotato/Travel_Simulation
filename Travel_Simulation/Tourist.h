#ifndef TOURIST_H
#define TOURIST_H

#include "Time.h"
#include "Strategy.h"

#include <string>

using namespace std;

class Tourist {
    unsigned short id;      // 乘客编号
    string depart;          // 出发地
    string dest;            // 目的地
    int type;               // 旅客选择的策略：
                            // 1:策略一，2:策略二，3：策略三
    Time expectedTime;      // 策略三中的限时
    string location;        // 旅客当前位置
public:
    Tourist(string start, string end, int strategy);
    Tourist(string start, string end, int strategy, Time t);
    unsigned short getId();
    string getDepart();
    string getDest();
    int getType();
    Time getExpectedTime();
    string getLocation();
    void getStrategy();
};

#endif // TOURIST_H
