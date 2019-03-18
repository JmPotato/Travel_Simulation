#include "Graph.h"

//成员函数的实现
Graph::Graph()
{
	string v1,v2;       //两个城市
	int w;              //权值（时间或价格）
	vexnum=12;
	
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


