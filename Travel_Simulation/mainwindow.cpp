#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Strategy.h"
#include "Tourist.h"

/**
 * @brief MainWindow::MainWindow
 * @param parent
 * @author ghz
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->startTime->setDateTime(QDateTime::currentDateTime());
    ui->expectedEndTime->setDateTime(QDateTime::currentDateTime());
    ui->endTime->setDateTime(QDateTime::currentDateTime());
    ui->endTime->setEnabled(false);
    ui->expectedEndTime->setEnabled(false);
    ui->budgetEdit->setEnabled(false);
    QStringList strategyList = {"最少费用", "最少用时", "最少费用+时间"};
    ui->strategyBox->addItems(strategyList);
    QStringList cityList = {"上海", "北京","南京" ,"广州" ,"成都" ,"杭州" ,"武汉" ,"深圳" ,"西安" ,"郑州" ,"重庆" ,"青岛"};
    ui->departureBox->addItems(cityList);
    ui->destinationBox->addItems(cityList);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::on_planButton_clicked
 * @author ghz
 */
void MainWindow::on_planButton_clicked() {
    if (ui->departureBox->currentText() == ui->destinationBox->currentText())
        ui->logBrowser->setText(QString("您的出发城市和到达城市一样，请重新选择"));
    else {
        MyTime startTime(0, ui->startTime->time().hour(), ui->startTime->time().minute());
        uint intervalTime = 0;
        unsigned short day;
        unsigned short hour;
        unsigned short minute;
        if (ui->expectedEndTime->dateTime().toTime_t() > ui->startTime->dateTime().toTime_t())
            intervalTime = ui->expectedEndTime->dateTime().toTime_t() - ui->startTime->dateTime().toTime_t();
        day = intervalTime / (24 * 60 * 60);
        hour = (intervalTime % (24 * 60 * 60)) / (60 * 60);
        minute = (intervalTime % (24 * 60 * 60)) % (60 * 60) / 60;
        MyTime period(day, hour, minute);
        MyTime expectedEndTime = startTime + period;
//        cout << expectedEndTime.day << "天" << expectedEndTime.hour << "时" << expectedEndTime.minute << "分\n";
        Tourist t(ui->departureBox->currentText().toStdString(), ui->destinationBox->currentText().toStdString(), startTime, expectedEndTime, ui->strategyBox->currentIndex() + 1);
        t.getStrategy();
        ui->logBrowser->setText(t.getLog());
        MyTime endTime = startTime + t.getPlanResult()->destTime - t.getPlanResult()->expectedDepartTime;
        ui->endTime->setDate(ui->startTime->date().addDays(endTime.day));
        ui->endTime->setTime(QTime::fromString("00:00", "hh:mm"));
        ui->endTime->setTime(ui->endTime->time().addSecs(endTime.hour * 3600 + endTime.minute * 60));
        int cost=t.getPlanResult()->result.moenyCost;
        ui->budgetEdit->setText(QString("RMB ¥")+QString::number(cost));
    }
}

void MainWindow::on_strategyBox_currentIndexChanged(int index) {
    if(index != 2)
        ui->expectedEndTime->setEnabled(false);
    else
        ui->expectedEndTime->setEnabled(true);
}
