#include "Tourist.h"

Tourist::Tourist(string start, string end, int strategy) {
//    id = 0;
    depart = start;
    dest = end;
    type = strategy;
    expectedTime.day = 0;
    expectedTime.hour = 0;
    expectedTime.minute = 0;    //初始化时无特殊指定，期望时间默认为 0
}

Tourist::Tourist(string start, string end, int strategy, Time t) {
//    id = 0;
    depart = start;
    dest = end;
    type = strategy;
    expectedTime = t;
}

unsigned short Tourist::getId() {
    return id;
}

string Tourist::getDepart() {
    return depart;
}

string Tourist::getDest() {
    return dest;
}

int Tourist::getType() {
    return type;
}

Time Tourist::getExpectedTime() {
    return expectedTime;
}

string Tourist::getLocation() {
    return location;
}

void Tourist::getStrategy() {
    Strategy touristStrategy(type, depart, dest, expectedTime);
    touristStrategy.startStrategy();
}
