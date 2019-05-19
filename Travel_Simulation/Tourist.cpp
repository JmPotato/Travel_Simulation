#include "Tourist.h"

/**
 * @brief Tourist::Tourist
 * @param start
 * @param end
 * @param strategy
 * @author ghz
 */
Tourist::Tourist()
{

}

Tourist::Tourist(string start, string end, int strategy) {
//    id = 0;
    depart = start;
    dest = end;
    type = strategy;
    expectedDestTime.day = 0;
    expectedDestTime.hour = 0;
    expectedDestTime.minute = 0;
}

/**
 * @brief Tourist::Tourist
 * @param start
 * @param end
 * @param startTime
 * @param strategy
 * @author ghz
 */
Tourist::Tourist(string start, string end, MyTime startTime, int strategy) {
//    id = 0;
    depart = start;
    dest = end;
    type = strategy;
    expectedDepartTime = startTime;
}

/**
 * @brief Tourist::Tourist
 * @param start
 * @param end
 * @param startTime
 * @param endTime
 * @param strategy
 * @author ghz
 */
Tourist::Tourist(string start, string end, MyTime startTime, MyTime endTime, int strategy) {
//    id = 0;
    depart = start;
    dest = end;
    type = strategy;
    expectedDepartTime = startTime;
    expectedDestTime = endTime;
}

Tourist::Tourist(string start, string end, QStringList Cities, vector<int> Hours, MyTime startTime, MyTime endTime, int strategy)
{
    depart = start;
    dest = end;
    passCity = Cities;
    passHours = Hours;
    type = strategy;
    expectedDepartTime = startTime;
    expectedDestTime = endTime;
}

/**
 * @brief Tourist::getId
 * @return
 * @author ghz
 */
unsigned short Tourist::getId() {
    return id;
}

/**
 * @brief Tourist::getDepart
 * @return
 * @author ghz
 */
string Tourist::getDepart() {
    return depart;
}

/**
 * @brief Tourist::getDest
 * @return
 * @author ghz
 */
string Tourist::getDest() {
    return dest;
}

/**
 * @brief Tourist::getType
 * @return
 * @author ghz
 */
int Tourist::getType() {
    return type;
}

/**
 * @brief Tourist::getLocation
 * @return
 * @author ghz
 */
string Tourist::getLocation() {
    return location;
}

/**
 * @brief Tourist::getStrategy
 * @author ghz
 */
void Tourist::getStrategy() {
    planResult = new Strategy(type, depart, dest, expectedDepartTime, expectedDestTime);
    planResult->startStrategy(log);
}

void Tourist::getPassStrategy()
{
    planResult = new Strategy(type, depart,dest, passCity, passHours,expectedDepartTime, expectedDestTime);
    planResult->startPassStrategy(log);
}

/**
 * @brief Tourist::getPlanResult
 * @return
 * @author ghz
 */
Strategy *Tourist::getPlanResult() {
    return planResult;
}

/**
 * @brief Tourist::getLog
 * @return
 * @author ghz
 */
QString Tourist::getLog() {
    return log;
}
