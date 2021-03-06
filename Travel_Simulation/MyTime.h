#ifndef MyTime_H
#define MyTime_H

#include <QDebug>
#include <QString>

#include <iostream>

class MyTime {
public:
    /* MyTime 类的基本结构 */
    unsigned short day;
    unsigned short hour;
    unsigned short minute;

    /* 构造与析构函数 */
    MyTime();
    MyTime(unsigned short d, unsigned short h, unsigned short m);
    MyTime(QString timeString);
    MyTime(const MyTime &obj);
    ~MyTime();

    /* 对加法、减法等符号重载 */
    MyTime operator+(const MyTime &t);
    MyTime operator-(const MyTime &t);
    bool operator==(const MyTime &t);
    bool operator<(const MyTime &t);
    bool operator>(const MyTime &t);

    /* 从字符串读入一个时间并格式化 */
    void parseString(QString timeString);
    void print();
};

#endif // MyTime_H
