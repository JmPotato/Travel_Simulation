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
    MyTime expectedDepartTime;        // 期望出发时间
    MyTime expectedDestTime;          // 期望到达时间
    string location;        // 旅客当前位置
    QString log;
    Strategy *planResult;
public:
    Tourist(string start, string end, int strategy);
    Tourist(string start, string end, MyTime startTime, int strategy);
    Tourist(string start, string end, MyTime startTime, MyTime endTime, int strategy);
    unsigned short getId();
    string getDepart();
    string getDest();
    int getType();
    string getLocation();
    void getStrategy();
    QString getLog();
    Strategy *getPlanResult();
};

#endif // TOURIST_H
