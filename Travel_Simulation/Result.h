#ifndef RESULT_H
#define RESULT_H

#include "Path.h"
#include "MyTime.h"

#include <vector>

using std::vector;

typedef struct RESULT {
    /* 某策略所需要的总的时间、金钱花费 */
    MyTime timeCost;
    MyTime destTime;
    int moenyCost;
    /* 存储某策略产生的一条从出发地到目的地的连续的路径 */
    vector<Path> route;
} Result;


#endif // RESULT_H
