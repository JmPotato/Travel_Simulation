#ifndef MyTime_H
#define MyTime_H

#include <iostream>
#include <QString>

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
    bool operator==(const MyTime &t);
    bool operator<(const MyTime &t);
    bool operator>(const MyTime &t);

    void parseString(QString timeString);
};

#endif // MyTime_H
