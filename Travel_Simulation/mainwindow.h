#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "Strategy.h"
#include "Tourist.h"

#include <QSet>
#include <QDate>
#include <QDebug>
#include <QQueue>
#include <QTimer>
#include <QString>
#include <QPainter>
#include <QDateTime>
#include <QTextStream>
#include <QMainWindow>
#include <QMessageBox>
#include <QStringList>
#include <QListWidgetItem>

#include <cmath>
#include <queue>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);
    void initCityPoint();
private slots:
    void changeDepartCity();
    void changeDestCity();
    void on_addCity_clicked();
    void on_deleteCity_clicked();
    void changeTravelStatus();
    void on_strategyBox_currentIndexChanged(int index);
    void on_planButton_clicked();
    void on_simButton_clicked();
    void on_pauseButton_clicked();
    void on_changePlanButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<QString> addedCities;
    QString departCity;
    QString destCity;
    QStringList cityList = {"上海", "北京","南京" ,"广州" ,"成都" ,"杭州" ,"武汉" ,"深圳" ,"西安" ,"郑州" ,"重庆" ,"青岛"};
    QPointF cityPoint[12];
    bool planReady;
    QTimer *ptimer;
    Tourist tourist;
    int currentMinute;              //模拟旅行中当前的分钟
    int totalMinutes;               //模拟旅行中总的分钟
    int day;                        //模拟旅行中当前的天数
    int pathIndex;                  //模拟旅行中当前已到达第几个path，第一个path为path1
    int cityIndex;                  //模拟旅行中当前已到达第几个city,出发城市是city0
    bool alreadyStart;              //判断是否已经开始模拟
    bool onPath;                    //判读是否正在一条path上
    int targetMinutes;              //用于判断是否到达一个path的开始
    int targetMinutes2;             //用于判断是否到达一个path的结束
    QString currentPathStart;
    QString currentPathEnd;
    int lastDepartMinute;
    int currentPeriodMinute;
    QVector<QPointF> allPassPoint;
    QQueue<int> pathStartMinutes;   //每条path的开始时间，转化为分钟
    QQueue<int> pathEndMinutes;     //每条path的结束时间，转化为分钟
    QVector<QString> cities;        //出发城市--->中间城市--->终点城市
    QVector<QString> pathes;        //path1--->path2--->...>pathN
    int alreadyPassCity;            //途经城市列表中已经经过的城市数量
};

#endif // MAINWINDOW_H
