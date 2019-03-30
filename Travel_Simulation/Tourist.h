#ifndef TOURIST_H
#define TOURIST_H

#include "MyTime.h"
#include "Strategy.h"

#include <string>

using namespace std;

class Tourist {
private:
    /* 由交互见面传来的某游客的信息 */
    unsigned short id;                  // 乘客编号（暂时未使用）
    string depart;                      // 出发地
    string dest;                        // 目的地
    int type;                           // 旅客选择的策略（1:策略一，2:策略二，3：策略三）
    MyTime expectedDepartTime;          // 期望出发时间
    MyTime expectedDestTime;            // 期望到达时间
    string location;                    // 旅客当前位置（暂时未使用）

    /* 产生的信息 */
    QString log;

    /* 用来调用策略 */
    Strategy *planResult;

public:
    /* 构造函数 */
    Tourist(string start, string end, int strategy);
    Tourist(string start, string end, MyTime startTime, int strategy);
    Tourist(string start, string end, MyTime startTime, MyTime endTime, int strategy);

    /* 一些也许可以用到的 getter 函数 */
    unsigned short getId();         // 未用
    string getDepart();
    string getDest();
    int getType();
    string getLocation();
    void getStrategy();
    QString getLog();
    Strategy *getPlanResult();      // 产生策略
};

#endif // TOURIST_H
