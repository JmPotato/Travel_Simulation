#include "Graph.h"

//成员函数的实现
Graph::Graph(int cityNum)
{
	string v1,v2;       //两个城市
	int w;              //权值（时间或价格）
	vexnum=cityNum;
	
	//动态申请内存
	matrix = new int*[vexnum];
	for(int i=0;i<vexnum;i++)
		matrix[i] = new int[vexnum];
	
	//均初始化为极大值MaxInt
	for(int i=0;i<vexnum;i++)
		for(int j=0;j<vexnum;j++)
		{
			matrix[i][j]=MaxInt;
		}
	//构造邻接矩阵
	for(int k=0;k<vexnum;k++)
	{
		cin>>v1>>v2>>w;
		i=locateVex(v1);
		j=locateVex(v1);
		matrix[i][j]=w;
	}		
}

//析构函数，释放内存
Graph::~Graph()
{
	for(int i=0;i<vexnum;i++)
		delete[] matrix[i];
	delete[] matrix;
}
int Graph::locateVex(string city)
{
	int index;
	for(index=0;index<vexnum;index++)
		if(vexs[index]==city)
			return index;
}

int Graph::getValue(string city1,string city2)
{
	int i=locateVex(city1);
	int j=LocateVex(city2);
	return matrix[i][j];
}

void Graph::setValue(string city1,string city2,int value)
{
	int i=locateVex(city1);
	int j=LocateVex(city2);
	matrix[i][j]=value;
}


