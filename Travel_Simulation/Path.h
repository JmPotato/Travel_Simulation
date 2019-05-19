#ifndef PATH_H
#define PATH_H

#include <string>
#include "MyTime.h"
#include <QString>

using std::string;

typedef  struct PATH {
    /* 本条路径的出发地与到达地 */
    string start;
    string end;

    /* 交通工具以及对应的航班号/车次 */
    QString tool;
    QString number;

    /* 本条路径的开始时间与结束时间 */
    MyTime startTime;
    MyTime endTime;

    /* 本条路径上时间与金钱的花费 */
    MyTime timeCost;
    int moneyCost;
} Path;

#endif // PATH_H
