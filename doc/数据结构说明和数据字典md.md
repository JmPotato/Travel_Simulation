# 数据结构说明和数据字典

## 1.时间结构体 Time
```cpp
typedef struct TIME {
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
} Time;
```
## 2.旅客类 Tourist

```cpp
class Tourist {
    
    unsigned short id;      // 乘客编号
    string depart;          // 出发地
    string dest;            // 目的地
    STRATEGY type;          // 旅客选择的策略：
                            // 1:策略一，2:策略二，3：策略三
    Time expectedTime;      // 策略三中的限时
    string location;        // 旅客当前位置
};
```

## 3.旅行路径 Path

```cpp
typedef struct PATH {
    string start;
    string end;
    Time timeCost;
    unsigned int moneyCost;
} Path;
```


## 4.策略结果结构体 Result

```cpp
typedef struct RESULT {
    Time timeCost;          // 花费时间
    unsigned int moneyCost; // 花费金钱
    vector<Path> route;    // 路线
} Result;
```

## 5.旅行策略类 Strategy

```cpp
class Strategy {
    STRATEGY type;          // 策略类型
    Result result;
};
```
## 6.图 Graph

```cpp
class Graph {
    int row, col;
    vector<string> vertex;
    int **matrix;
};
```


