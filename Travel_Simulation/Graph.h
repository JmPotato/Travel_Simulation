#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include <vector>
using std::vector;
using std::string;
#define MaxInt 32767

//图类的定义
class Graph 
{
private:
    vector<string> vexs;              //顶点表
    int **matrix;                     //邻接矩阵
    unsigned long vexnum;                        //顶点数
public:
    Graph(unsigned long cityNum);                      //构造函数,参数是城市的数量
	~Graph();
    long locateVex(string city);
	void ShortestPath_DIJ();
    int getValue(string city1,string city2);
    void setValue(string city1,string city2,int value);
    void setVexsList(vector<string> list);
    void printMatrix();
};

#endif //GRAPH_H

//采用带权有向网，每条边的权值是钱或者价格，
//因为是稠密图（边数比较多），所若采用邻接表存储图，空间复杂度较高（O(n+e)）。所以宜采用邻接矩阵来存储图
