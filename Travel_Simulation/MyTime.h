#ifndef MyTime_H
#define MyTime_H

#include <iostream>
#include <QString>

class MyTime {
public:
    /* MyTime 类的基本结构 */
    unsigned short day;
    unsigned short hour;
    unsigned short minute;

    /* 构造与析构函数 */
    MyTime();
    MyTime(unsigned short d, unsigned short h, unsigned short m);
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
};

#endif // MyTime_H
