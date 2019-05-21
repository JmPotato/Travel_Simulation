#include "MyTime.h"

/**
 * @brief normalizeTime
 * @param d
 * @param h
 * @param m
 * @author ghz
 */
void normalizeTime(unsigned short &d, unsigned short &h, unsigned short &m) {
    unsigned short overHour = 0;
    unsigned short overDay = 0;
    if(m >= 60) {
        overHour = m/60;
        m %= 60;
    }
    h += overHour;
    if(h >= 24) {
        overDay = h/24;
        h %= 24;
    }
    d += overDay;
}

/**
 * @brief MyTime::MyTime
 * @author ghz
 */
MyTime::MyTime() {
    minute = 0;
    day = 0;
    hour = 0;
}

/**
 * @brief MyTime::MyTime
 * @param d
 * @param h
 * @param m
 * @author ghz
 */
MyTime::MyTime(unsigned short d, unsigned short h, unsigned short m) {
    minute = m;
    hour = h;
    day = d;
    normalizeTime(day, hour, minute);
}

/**
 * @brief MyTime::MyTime
 * @param timeString
 * @author hzy
 */
MyTime::MyTime(QString timeString) {
    day = 0;
    hour = timeString.section(":", 0, 0).toUShort();
    minute = timeString.section(":", 1, 1).toUShort();
    normalizeTime(day, hour, minute);
}

/**
 * @brief MyTime::MyTime
 * @param obj
 * @author ghz
 */
MyTime::MyTime(const MyTime &obj) {
    this->minute = obj.minute;
    this->hour = obj.hour;
    this->day = obj.day;
    normalizeTime(this->day, this->hour, this->minute);
}

/**
 * @brief MyTime::~MyTime
 */
MyTime::~MyTime() {

}

/**
 * @brief MyTime::operator +
 * @param t
 * @return
 * @author ghz
 */
MyTime MyTime::operator+(const MyTime &t) {
    MyTime newMyTime(this->day+t.day, this->hour+t.hour, this->minute+t.minute);
    return newMyTime;
}

/**
 * @brief MyTime::operator -
 * @param t
 * @return
 * @author ghz
 */
MyTime MyTime::operator-(const MyTime &t) {
    unsigned thisSumMinute = this->day * 1440 + this->hour * 60 + this->minute;
    unsigned tSumMinute = t.day * 1440 + t.hour * 60 + t.minute;
    if(thisSumMinute < tSumMinute)
        std::cerr << "Illegal Time Operation" << std::endl;
    MyTime newMyTime(0, 0, thisSumMinute-tSumMinute);
    return newMyTime;
}

/**
 * @brief MyTime::operator ==
 * @param t
 * @return
 * @author ghz
 */
bool MyTime::operator==(const MyTime &t) {
    unsigned thisSumMinute = this->day * 1440 + this->hour * 60 + this->minute;
    unsigned tSumMinute = t.day * 1440 + t.hour * 60 + t.minute;
    return (thisSumMinute == tSumMinute);
}

/**
 * @brief MyTime::operator <
 * @param t
 * @return
 * @author ghz
 */
bool MyTime::operator<(const MyTime &t) {
    unsigned thisSumMinute = this->day * 1440 + this->hour * 60 + this->minute;
    unsigned tSumMinute = t.day * 1440 + t.hour * 60 + t.minute;
    return (thisSumMinute < tSumMinute);
}

/**
 * @brief MyTime::operator >
 * @param t
 * @return
 * @author ghz
 */
bool MyTime::operator>(const MyTime &t) {
    unsigned thisSumMinute = this->day * 1440 + this->hour * 60 + this->minute;
    unsigned tSumMinute = t.day * 1440 + t.hour * 60 + t.minute;
    return (thisSumMinute > tSumMinute);
}

/**
 * @brief MyTime::parseString
 * @param timeString
 * @author ghz
 */
void MyTime::parseString(QString timeString) {
    day = 0;
    hour = timeString.section(":", 0, 0).toUShort();
    minute = timeString.section(":", 1, 1).toUShort();
    normalizeTime(day, hour, minute);
}

void MyTime::print()
{
    qDebug()<<day << "天" << hour << "时" << minute << "分";
}
