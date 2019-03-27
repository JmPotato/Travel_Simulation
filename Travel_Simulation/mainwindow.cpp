#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Strategy.h"
#include "Tourist.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->startTime->setDate(QDate::currentDate());
    ui->endTime->setEnabled(false);
    ui->budgetEdit->setEnabled(false);
    QStringList strategyList = {"最少费用", "最少用时", "最少费用+时间"};
    ui->strategyBox->addItems(strategyList);
    QStringList cityList = {"上海", "北京","南京" ,"广州" ,"成都" ,"杭州" ,"武汉" ,"深圳" ,"西安" ,"郑州" ,"重庆" ,"青岛"};
    ui->departureBox->addItems(cityList);
    ui->destinationBox->addItems(cityList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_planButton_clicked() {
    MyTime startTime(0, ui->startTime->time().hour(), ui->startTime->time().minute());
    Tourist t(ui->departureBox->currentText().toStdString(), ui->destinationBox->currentText().toStdString(), startTime, ui->strategyBox->currentIndex() + 1);
    t.getStrategy();
    ui->logBrowser->setText(t.getLog());
    MyTime endTime = startTime + t.getPlanResult()->destTime - t.getPlanResult()->expectedDepartTime;
    ui->endTime->setDate(ui->startTime->date().addDays(endTime.day));
    ui->endTime->setTime(QTime::fromString("00:00", "hh:mm"));
    ui->endTime->setTime(ui->endTime->time().addSecs(endTime.hour * 3600 + endTime.minute * 60));
}
