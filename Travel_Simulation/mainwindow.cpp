#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Strategy.h"
#include "Tourist.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->endTime->setEnabled(false);
    ui->startTime->setDate(QDate::currentDate());
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
    Tourist t(ui->departureBox->currentText().toStdString(), ui->destinationBox->currentText().toStdString(), startTime, 1);
    t.getStrategy();
    ui->logBrowser->setText(t.getLog());
    QDateTime desTime = ui->startTime->dateTime();
    MyTime usedTime = t.getPlanResult()->destTime - t.getPlanResult()->expectedDepartTime;
    desTime.setDate(desTime.date().addDays(usedTime.day));
    desTime.setTime(desTime.time().addSecs((usedTime.hour * 60 + usedTime.minute)*60));
    ui->endTime->setDateTime(desTime);
}
