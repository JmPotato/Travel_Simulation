#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDate>
#include <QDateTime>
#include <QTextStream>
#include <QMainWindow>
#include <QSet>
#include <QString>
#include <QTimer>
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

private:
    Ui::MainWindow *ui;
    QList<QString> addedCities;
    QString departCity;
    QString destCity;
    bool planReady;
    QTimer *ptimer;
    Tourist tourist;
    int currentMinute;
    int day; //模拟旅行中用到的天数
};

#endif // MAINWINDOW_H
