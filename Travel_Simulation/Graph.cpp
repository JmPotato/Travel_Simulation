#include "Graph.h"
#include <iostream>
#include <QStack>
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

    timeTable = new MyTime*[vexnum];
    for(unsigned long i = 0; i < vexnum; i++)
        timeTable[i] = new MyTime[vexnum];

    //均初始化为极大值MaxInt
    for(unsigned long  i = 0; i < vexnum; i++)
        for(unsigned long j = 0;j < vexnum; j++)
        {
            matrix[i][j] = MaxInt;
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

MyTime Graph::getTimeTableValue(std::string city1, std::string city2)
{
    long i = locateVex(city1);
    long j = locateVex(city2);
    return timeTable[i][j];
}

std::string Graph::getVex(long i)
{
    return vexs[i];
}

void Graph::setValue(string city1,string city2,int value)
{
    long i = locateVex(city1);
    long j = locateVex(city2);
    matrix[i][j] = value;
}

void Graph::setTimeTableValue(std::string city1, std::string city2, MyTime value)
{
    long i = locateVex(city1);
    long j = locateVex(city2);
    timeTable[i][j] = value;
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

void Graph::shortestPathDJ(string Dep, string Dest, Result &result)
{
    long *tempPath = new long[vexnum];
    bool *s = new bool[vexnum];
    int *d = new int[vexnum];
    // Dijkstra Algorithm
    unsigned long n = vexnum;
    long v0 = locateVex(Dep);
    long vt = locateVex(Dest);
    unsigned long w, v;
    for (v = 0; v < n; v++) {
        s[v] = false;
        d[v] = matrix[v0][v];
        if (d[v] < MaxInt) tempPath[v] = v0;
        else tempPath[v] = -1;
    }
    s[v0] = true;
    d[v0] = 0;
    int min;
    for (unsigned long i = 1; i < n; i++) {
        min = MaxInt;
        for (w = 0; w < n; w++)
            if (!s[w] && d[w] < min) {
                v = w;
                min = d[w];
            }
        s[v] = true;
        for (w = 0; w < n; w++)
            if (!s[w] && (d[v] + matrix[v][w] < d[w])) {
                d[w] = d[v] + matrix[v][w];
                tempPath[w] = v;
            }
    }

    Path path;

    while (tempPath[vt] != -1) {
        long pre = tempPath[vt];
        path.start = getVex(pre);
        path.end = getVex(vt);
        path.moneyCost = matrix[pre][vt];
        result.route.push_back(path);
        vt = pre;
    }

    vt = locateVex(Dest);
    result.moenyCost = d[vt];
    std::reverse(result.route.begin(), result.route.end());

    delete [] tempPath;
    delete [] s;
    delete [] d;

}
//city的第一个可访问的邻接点
int Graph::findNext(QString city)
{
    for(unsigned long i=0;i<vexnum;i++)
    {
        if (!visited[locateVex(city.toStdString())][i + 1] && !visited[i][0])
           return i;
    }
    return -1;
}

//找到两个城市之间的所有路径，保存到allPath中，
void Graph::findAllPath(QString Dep, QString Dest)
{
    visited = new bool*[vexnum];
    for(unsigned long i=0;i<vexnum;i++)
        visited[i]=new bool[vexnum+1];

    QStack<QString> s;
    s.push(Dep);
    visited[locateVex(Dep.toStdString())][0]=true;
    QString top;
    int n;
    QString next;

    while(!s.empty())
    {
        top=s.top();
        if(top==Dest)
        {
            //将一条路径保存到allPath
            vector<QString> onePath;
            QStack<QString> temp = s;
            while (!temp.empty())
            {
                QString _city = s.top();
                onePath.push_back(_city);
                temp.pop();
            }
            reverse(onePath.begin(), onePath.end());
            allPath.push_back(onePath);

            s.pop();
            visited[locateVex(top.toStdString())][0] = false;
        }
        else
        {
            n =findNext(top);
            next = QString::fromStdString(vexs[n]);
            if (n != -1 && s.size()<4)
            {
               s.push(next);
               visited[locateVex(top.toStdString())][n + 1] = true;
               visited[n][0] = true;
            }
            else
            {
               s.pop();
               for (unsigned long i = 0; i < vexnum+1; i++)
                 visited[locateVex(top.toStdString())][i] = false;
            }
        }
    }
   for(unsigned long i=0;i<vexnum;i++)
       delete[] visited[i];
   delete visited;

}


