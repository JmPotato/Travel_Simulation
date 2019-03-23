#ifndef RESULT_H
#define RESULT_H

#include <vector>
#include "MyTime.h"
#include "Path.h"

using std::vector;

typedef struct RESULT {
    MyTime timeCost;
    int moenyCost;
    vector<Path> route;
} Result;

#endif // RESULT_H
