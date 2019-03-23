#include "MyTime.h"

MyTime::MyTime() {
    minute = 0;
    day = 0;
    hour = 0;
}

MyTime::MyTime(unsigned short d, unsigned short h, unsigned short m) {
    minute = m;
    unsigned short overHour = 0;
    unsigned short overDay = 0;
    if(minute >= 60) {
        overHour = minute/60;
        minute %= 60;
    }
    hour = h + overHour;
    if(hour >= 24) {
        overDay = hour/24;
        hour %= 24;
    }
    day = d + overDay;
}

MyTime::MyTime(const MyTime &obj) {
    this->minute = obj.minute;
    unsigned short overHour = 0;
    unsigned short overDay = 0;
    if(this->minute >= 60) {
        overHour = this->minute/60;
        this->minute %= 60;
    }
    this->hour = obj.hour + overHour;
    if(this->hour >= 24) {
        overDay = this->hour/24;
        this->hour %= 24;
    }
    this->day = obj.day + overDay;
}

MyTime::~MyTime() {

}

MyTime MyTime::operator+(const MyTime &t) {
    MyTime newMyTime(this->day+t.day, this->hour+t.hour, this->minute+t.minute);
    return newMyTime;
}

MyTime MyTime::operator-(const MyTime &t) {
    unsigned short thisSumMinute = this->day * 1440 + this->hour * 60 + this->minute;
    unsigned short tSumMinute = t.day * 1440 + t.hour * 60 + t.minute;
    if(thisSumMinute < tSumMinute)
        std::cerr << "Illegal Time Operation" << std::endl;
    MyTime newMyTime(0, 0, thisSumMinute-tSumMinute);
    return newMyTime;
}
