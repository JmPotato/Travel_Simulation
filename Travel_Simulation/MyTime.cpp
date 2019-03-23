#include "MyTime.h"

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


MyTime::MyTime() {
    minute = 0;
    day = 0;
    hour = 0;
}

MyTime::MyTime(unsigned short d, unsigned short h, unsigned short m) {
    minute = m;
    hour = h;
    day = d;
    normalizeTime(day, hour, minute);
}

MyTime::MyTime(const MyTime &obj) {
    this->minute = obj.minute;
    this->hour = obj.hour;
    this->day = obj.day;
    normalizeTime(this->day, this->hour, this->minute);
}

MyTime::~MyTime() {

}

MyTime MyTime::operator+(const MyTime &t) {
    MyTime newMyTime(this->day+t.day, this->hour+t.hour, this->minute+t.minute);
    return newMyTime;
}

MyTime MyTime::operator-(const MyTime &t) {
    unsigned thisSumMinute = this->day * 1440 + this->hour * 60 + this->minute;
    unsigned tSumMinute = t.day * 1440 + t.hour * 60 + t.minute;
    if(thisSumMinute < tSumMinute)
        std::cerr << "Illegal Time Operation" << std::endl;
    MyTime newMyTime(0, 0, thisSumMinute-tSumMinute);
    return newMyTime;
}

bool MyTime::operator==(const MyTime &t) {
    unsigned thisSumMinute = this->day * 1440 + this->hour * 60 + this->minute;
    unsigned tSumMinute = t.day * 1440 + t.hour * 60 + t.minute;
    return (thisSumMinute == tSumMinute);
}

bool MyTime::operator<(const MyTime &t) {
    unsigned thisSumMinute = this->day * 1440 + this->hour * 60 + this->minute;
    unsigned tSumMinute = t.day * 1440 + t.hour * 60 + t.minute;
    return (thisSumMinute < tSumMinute);
}

bool MyTime::operator>(const MyTime &t) {
    unsigned thisSumMinute = this->day * 1440 + this->hour * 60 + this->minute;
    unsigned tSumMinute = t.day * 1440 + t.hour * 60 + t.minute;
    return (thisSumMinute > tSumMinute);
}

void MyTime::parseString(QString timeString) {
    day = 0;
    hour = timeString.section(":", 0, 0).toUShort();
    minute = timeString.section(":", 1, 1).toUShort();
    normalizeTime(day, hour, minute);
}
