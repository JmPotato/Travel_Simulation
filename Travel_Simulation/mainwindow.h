#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDate>
#include <QDateTime>
#include <QTextStream>
#include <QMainWindow>
#include <QSet>
#include <QString>
#include <QTimer>
#include <QQueue>
#include "Tourist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_planButton_clicked();

    void on_strategyBox_currentIndexChanged(int index);

    void on_addCity_clicked();

    void on_deleteCity_clicked();
    void changeDepartCity();
    void changeDestCity();
    void on_simButton_clicked();
    void changeTravelStatus();

    void on_pauseButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<QString> addedCities;
    QString departCity;
    QString destCity;
    bool planReady;
    QTimer *ptimer;
    Tourist tourist;
    int currentMinute;              //模拟旅行中当前的分钟
    int day;                        //模拟旅行中当前的天数
    int pathIndex;                  //模拟旅行中当前已到达第几个path，第一个path为path1
    int cityIndex;                  //模拟旅行中当前已到达第几个city,出发城市是city0
    bool alreadyStart;              //判断是否已经开始模拟
    int targetMinutes;              //用于判断是否到达一个path的开始
    int targetMinutes2;             //用于判断是否到达一个path的结束
    QQueue<int> pathStartMinutes;   //每条path的开始时间，转化为分钟
    QQueue<int> pathEndMinutes;     //每条path的结束时间，转化为分钟
    QVector<QString> cities;        //出发城市--->中间城市--->终点城市
    QVector<QString> pathes;        //path1--->path2--->...>pathN

};

#endif // MAINWINDOW_H
