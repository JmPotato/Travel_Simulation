# Travel_Simulation

旅行模拟查询系统的设计

## 1.问题描述

城市之间有三种交通工具（汽车、火车和飞机）相连，某旅客于某一时刻向系统提出旅行要求，系统根据该乘客的要求为其设计一条旅行线路并输出；系统能查询当前时刻旅客所处的地点和状态（停留城市/所在交通工具）。

## 2.功能需求

* 城市总数不少于 10 个
* 建立汽车、火车和飞机的时刻表（航班表）
    - 有沿途到站及票价信息
    - 不能太简单（不能总是 1 班车次相连）
* 旅客的要求包括：起点、终点、出发时间范围、途径某些城市和旅行策略
* 旅行策略有：
    - 最少费用策略：无时间限制，费用最少即可
    - 最少时间策略：无费用限制，时间最少即可
    - 现实最少费用策略：在规定的时间内所需费用最省
* 旅行模拟查询系统以时间为轴向前推移，每 10 秒左右向前推进 1 个小时（非查询状态的请求不计时）
* 不考虑城市内换乘交通工具所需时间
* 系统时间精确到小时
* 建立日志文件，对旅客状态变化和键入等信息进行记录
* **选做一：某旅客在旅行途中可更改旅行计划，系统应作出相应的操作**
* **选做二：用图形绘制地图，并在地图上反映出旅客的旅行过程**

## 3.程序参考结构

* 主模块
    - 接受键盘键入命令，分析该命令并调用相应的模块，并以时间为轴向前推进
* 旅游线路设计和输出模块
    - 生成相应的旅游线路
* 状态动态查询显示模块
* 日志文件处理模块
    - 完成相应的日志文件写入和查询结果输出等功能

## 4.参考数据结构

* 汽车、火车和飞机的时刻表（航班表）（起点、时间）和）终点、时间）及票价
* 旅客状态表：旅客代码、当前状态
* 旅行计划表：旅客代码、旅行计划
* 日志文件：当前时间、事件

# Process Plan

* ~~Week 1 布置题目~~
* ~~Week 2 提交分组信息~~
* ~~Week 3 讲解题目~~
* ~~Week 4 提交项目计划~~
* Week 8 中期检查
* Week 15, 16 验收
* Every Week 发周报到邮箱 zhyprj@sina.com

2019.3.4 前提交分组情况；2019.6.4 提交电子版报告。

# Document Plan

* 设计任务的描述
* 功能需求说明及分析
* 总体方案设计说明（软件开发环境、总体结构和模块划分等）
* 数据结构说明和数据字典（数据名称、用途等）
* 各模块设计说明（算法思想、算法、特点及其它模块的关系等）
* 范例执行结果及测试情况说明
* 评价和改进意见
* 用户使用说明

# Reference
* [Qt快速入门](http://www.qter.org/forum.php?mod=viewthread&tid=193)
* [疑似是以前学长做的](https://www.write-bug.com/article/1888.html)
* [Efficient Point-to-Point Shortest
Path Algorithms](http://www.cs.princeton.edu/courses/archive/spr06/cos423/Handouts/EPP%20shortest%20path%20algorithms.pdf)
* [A* 搜索算法有哪些变种？
](https://zhuanlan.zhihu.com/p/45526578)
* [Search-Based Planning Library
](https://github.com/sbpl/sbpl)
* [16-782 Planning and Decision-making in Robotics
](http://www.cs.cmu.edu/~maxim/classes/robotplanning_grad)

