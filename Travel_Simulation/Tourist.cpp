#include "Tourist.h"

Tourist::Tourist(string start, string end, int strategy) {
//    id = 0;
    depart = start;
    dest = end;
    type = strategy;
}

Tourist::Tourist(string start, string end, MyTime startTime, int strategy) {
//    id = 0;
    depart = start;
    dest = end;
    type = strategy;
    expectedDepartTime = startTime;
}

Tourist::Tourist(string start, string end, MyTime startTime, MyTime endTime, int strategy) {
//    id = 0;
    depart = start;
    dest = end;
    type = strategy;
    expectedDepartTime = startTime;
    expectedDestTime = endTime;
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

string Tourist::getLocation() {
    return location;
}

void Tourist::getStrategy() {
    planResult = new Strategy(type, depart, dest, expectedDepartTime, expectedDestTime);
    planResult->startStrategy(log);
}


Strategy *Tourist::getPlanResult() {
    return planResult;
}

QString Tourist::getLog() {
    return log;
}
