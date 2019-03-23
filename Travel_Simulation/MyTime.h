#ifndef MyTime_H
#define MyTime_H

#include <iostream>

//typedef  struct MyTime {
//    unsigned short day;
//    unsigned short hour;
//    unsigned short minute;
//} MyTime;

class MyTime {
public:
    unsigned short day;
    unsigned short hour;
    unsigned short minute;

    MyTime();
    MyTime(unsigned short d, unsigned short h, unsigned short m);
    MyTime(const MyTime &obj);
    ~MyTime();

    MyTime operator+(const MyTime &t);
    MyTime operator-(const MyTime &t);
};

#endif // MyTime_H
