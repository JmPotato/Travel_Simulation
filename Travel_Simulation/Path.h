#ifndef PATH_H
#define PATH_H

#include <string>
#include "MyTime.h"
#include <QString>

using std::string;

typedef  struct PATH {
    string start;
    string end;
    QString number;
    MyTime timeCost;
    MyTime startTime;
    MyTime endTime;
    int moneyCost;
} Path;

#endif // PATH_H
