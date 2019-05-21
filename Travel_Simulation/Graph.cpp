#include "Graph.h"

using std::cout;
using std::endl;

/**
 * @brief Graph::Graph
 * @param cityNum
 * @author hyd
 *
 * 实例化一个 Graph 类，为它里面的结构分配内存，并初始化
 */
Graph::Graph(unsigned long cityNum)
{
    vexnum = cityNum;
	
    // 动态申请内存
    matrix = new int*[vexnum];
    for(unsigned long i = 0; i < vexnum; i++)
        matrix[i] = new int[vexnum];

    timeTable = new MyTime*[vexnum];
    for(unsigned long i = 0; i < vexnum; i++)
        timeTable[i] = new MyTime[vexnum];

    // 均初始化为极大值MaxInt
    for(unsigned long  i = 0; i < vexnum; i++)
        for(unsigned long j = 0;j < vexnum; j++)
        {
            matrix[i][j] = MaxInt;
        }
}

/**
 * @brief Graph::~Graph
 * @author hyd
 *
 * 释放类中分配了的内存
 */
Graph::~Graph()
{
    for(unsigned long i = 0; i < vexnum;i++)
		delete[] matrix[i];
    delete[] matrix;
}

/**
 * @brief Graph::locateVex
 * @param city
 * @return 某城市对应的序号
 * @author hyd
 */
long Graph::locateVex(string city)
{
    vector<string>::iterator index;
    for (index = vexs.begin(); index != vexs.end(); index++ )
        if (*index == city)
            return (index - vexs.begin());
    return 0;
}

/**
 * @brief Graph::getValue
 * @param city1
 * @param city2
 * @return 邻接矩阵中某点值
 * @author hyd
 */
int Graph::getValue(string city1,string city2)
{
    long i = locateVex(city1);
    long j = locateVex(city2);
    return matrix[i][j];
}

/**
 * @brief Graph::getTimeTableValue
 * @param city1
 * @param city2
 * @return 时间矩阵某店的值
 * @author hzy
 */
MyTime Graph::getTimeTableValue(std::string city1, std::string city2)
{
    long i = locateVex(city1);
    long j = locateVex(city2);
    return timeTable[i][j];
}

/**
 * @brief Graph::getVex
 * @param i
 * @return 序号i对应的某城市
 * @author hzy
 */
std::string Graph::getVex(long i)
{
    return vexs[i];
}

/**
 * @brief Graph::setValue
 * @param city1
 * @param city2
 * @param value
 * @author hyd
 */
void Graph::setValue(string city1,string city2,int value)
{
    long i = locateVex(city1);
    long j = locateVex(city2);
    matrix[i][j] = value;
}

/**
 * @brief Graph::setTimeTableValue
 * @param city1
 * @param city2
 * @param value
 * @author hzy
 */
void Graph::setTimeTableValue(std::string city1, std::string city2, MyTime value)
{
    long i = locateVex(city1);
    long j = locateVex(city2);
    timeTable[i][j] = value;
}

/**
 * @brief Graph::setVexsList
 * @param list
 * @author hzy
 */
void Graph::setVexsList(vector<std::string> list)
{
    vexs = list;
}

/**
 * @brief Graph::printMatrix
 * @author hzy
 */
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

/**
 * @brief Graph::shortestPathDJ
 * @param Dep
 * @param Dest
 * @param result
 * @author hzy
 *
 * 策略一所用的最短路算法
 */
void Graph::shortestPathDJ(string Dep, string Dest, Result &result)
{
    /* 需要用到的数据 */
    unsigned long w, v;                 // 临时变量
    int min;                            // 通过迭代算出来的最小值
    Path path;                          // 临时存储一条路径
    long *tempPath = new long[vexnum];  // 临时存储
    bool *s = new bool[vexnum];         // 辅助数组
    int *d = new int[vexnum];           // 辅助数组
    unsigned long n = vexnum;           // 辅助变量
    long v0 = locateVex(Dep);
    long vt = locateVex(Dest);

    // Dijkstra Algorithm
    /* 初始化 */
    for (v = 0; v < n; v++) {
        s[v] = false;
        d[v] = matrix[v0][v];
        if (d[v] < MaxInt) tempPath[v] = v0;
        else tempPath[v] = -1;
    }
    s[v0] = true;
    d[v0] = 0;

    /* 最短路算法运算部分 */
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

    /* 提取路径 */
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

    /* 释放内存 */
    delete [] tempPath;
    delete [] s;
    delete [] d;
}
