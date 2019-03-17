#ifndef RESULT_H
#define RESULT_H

#include <vector>
#include "Time.h"
#include "Path.h"

using std::vector;

typedef struct RESULT {
    Time timeCost;
    unsigned int moenyCost;
    vector<Path> route;
} Result;

#endif // RESULT_H
