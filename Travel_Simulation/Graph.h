#ifndef GRAPH_H
#define GRAPH_H

#include "Result.h"
#include "MyTime.h"

#include <string>
#include <vector>
#include <iostream>

#define MaxInt 32767        // Dijkstra Algorithm 中设定的无限大的值

using std::vector;
using std::string;

// 图类的定义
class Graph 
{
private:
    /* 策略一 */
    /* 一个有向图 */
    unsigned long vexnum;       // 顶点数
    vector<string> vexs;        // 顶点表
    int **matrix;               // 邻接矩阵
    MyTime **timeTable;         // 与上面邻接矩阵同构的各个路径的出发时间表

    /* 需要用到的 getter */
    string getVex(long i);      // 返回序号对应的城市
    long locateVex(string city);// 返回城市对应的序号

public:
    Graph(unsigned long cityNum);   // 构造函数,参数是城市的数量
    ~Graph();                       // 析构函数

    /* 需要用到的 getter 函数 */
    int getValue(string city1,string city2);                // 返回邻接矩阵某个点的值
    MyTime getTimeTableValue(string city1, string sity2);   // 返回 MyTime 矩阵某个点的值

    /* 需要用到的 setter 函数 */
    void setValue(string city1,string city2,int value);     // 为邻接矩阵某个点赋值
    void setTimeTableValue(string city1, string city2, MyTime value);   // 为 MyTime 矩阵某个点赋值
    void setVexsList(vector<string> list);                  // 传入城市表

    /* Dijkstra 算法求最短路径，将结果存在 result（目前来看是暂时）中 */
    void shortestPathDJ(string Dep, string Dest, Result &result);

    /* 打印邻接矩阵，用于产生调试信息 */
    void printMatrix();
};

#endif //GRAPH_H
