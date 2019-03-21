#include "Graph.h"
#include <iostream>
using std::cout;
using std::endl;
//成员函数的实现
Graph::Graph(unsigned long cityNum)
{
    //string v1,v2;       //两个城市
    //int w;              //权值（时间或价格）
    vexnum = cityNum;
	
	//动态申请内存
	matrix = new int*[vexnum];
    for(unsigned long i = 0; i < vexnum; i++)
		matrix[i] = new int[vexnum];
	
    //均初始化为极大值MaxInt
    for(unsigned long  i = 0; i < vexnum; i++)
        for(unsigned long j = 0;j < vexnum; j++)
        {
            matrix[i][j]=MaxInt;
        }
	//构造邻接矩阵
//	for(int k=0;k<vexnum;k++)
//	{
//		cin>>v1>>v2>>w;
//		i=locateVex(v1);
//		j=locateVex(v1);
//		matrix[i][j]=w;
//	}
}

//析构函数，释放内存
Graph::~Graph()
{
    for(unsigned long i = 0; i < vexnum;i++)
		delete[] matrix[i];
    delete[] matrix;
}
long Graph::locateVex(string city)
{

//	int index;
//	for(index = 0; index < vexnum;index++)
//		if(vexs[index] == city)
//			return index;
    vector<string>::iterator index;
    for (index = vexs.begin(); index != vexs.end(); index++ )
        if (*index == city)
            return (index - vexs.begin());
    return 0;
}

int Graph::getValue(string city1,string city2)
{
    long i = locateVex(city1);
    long j = locateVex(city2);
	return matrix[i][j];
}

void Graph::setValue(string city1,string city2,int value)
{
    long i = locateVex(city1);
    long j = locateVex(city2);
    matrix[i][j] = value;
}

void Graph::setVexsList(vector<std::string> list)
{
    vexs = list;
}

void Graph::printMatrix()
{
    for (unsigned long i = 0; i < vexnum; i++) {
        cout << i << ": " ;
        for (unsigned long j = 0; j < vexnum; j++) {
            cout << matrix[i][j] << '\t';
        }
        cout << endl;
    }
}


