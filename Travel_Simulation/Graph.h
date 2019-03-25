#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include <vector>
#include "Result.h"
#include "MyTime.h"
using std::vector;
using std::string;
#define MaxInt 32767


//图类的定义
class Graph 
{
private:
    vector<string> vexs;              //顶点表
    int **matrix;                     //邻接矩阵
    bool **visited;
    MyTime **timeTable;
    unsigned long vexnum;                        //顶点数
    string getVex(long i);
    long locateVex(string city);
public:
    vector<vector<QString>> allPath;    //策略二中所有路径
    Graph(unsigned long cityNum);                      //构造函数,参数是城市的数量
	~Graph();
    int getValue(string city1,string city2);
    MyTime getTimeTableValue(string city1, string sity2);
    void setValue(string city1,string city2,int value);
    void setTimeTableValue(string city1, string city2, MyTime value);
    void setVexsList(vector<string> list);
    void printMatrix();
    void shortestPathDJ(string Dep, string Dest, Result &result);
    void findAllPath(QString Dep, QString Dest);
    int findNext(QString city);
};

#endif //GRAPH_H

//采用带权有向网，每条边的权值是钱或者价格，
//因为是稠密图（边数比较多），所若采用邻接表存储图，空间复杂度较高（O(n+e)）。所以宜采用邻接矩阵来存储图
