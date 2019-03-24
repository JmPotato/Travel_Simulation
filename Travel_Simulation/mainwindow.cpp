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
    QStringList cityList = {"上海", "北京","南京" ,"广州" ,"成都" ,"杭州" ,"武汉" ,"深圳" ,"西安" ,"郑州" ,"重庆" ,"青岛"};
    ui->departureBox->addItems(cityList);
    ui->destinationBox->addItems(cityList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_planButton_clicked() {
    Tourist t(ui->departureBox->currentText().toStdString(), ui->destinationBox->currentText().toStdString(), 1);
    t.getStrategy();
    ui->logBrowser->setText(t.log);
}
