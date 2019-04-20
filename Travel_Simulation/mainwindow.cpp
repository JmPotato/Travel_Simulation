#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Strategy.h"
#include "Tourist.h"
#include <QDebug>
#include <QListWidgetItem>
#include <QStringList>

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
    QStringList addCityList = {"请选择途经城市", "上海", "北京","南京" ,"广州" ,"成都" ,"杭州" ,"武汉" ,"深圳" ,"西安" ,"郑州" ,"重庆" ,"青岛"};
    ui->departureBox->addItems(cityList);
    ui->destinationBox->addItems(cityList);
    ui->cityBox->addItems(addCityList);

    ui->tabWidget->setCurrentIndex(0);
    QImage mapImage("China-O.jpg");
    mapImage  = mapImage.scaled(QSize(800, 1000), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->mapBrowser->setPixmap(QPixmap::fromImage(mapImage));
    ui->mapBrowser->setScaledContents(true);

    connect(this->ui->departureBox,SIGNAL(currentTextChanged(QString)),this,SLOT(changeDepartCity()));
    connect(this->ui->destinationBox,SIGNAL(currentTextChanged(QString)),this,SLOT(changeDestCity()));
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
        int count = ui->passList->count();
        QStringList passCity;
        vector<int> hours;
        if(count > 1)
        {
            for(int i=1;i < count; i++)
            {
              QListWidgetItem *temp = ui->passList->item(i);
              QString tempText = (*temp).text();
              QString hoursString,cityString;
              for(int i = 0;i < 2; i++)
              {
                  cityString.append(tempText[i]);
              }
              for(int j = 0; j < tempText.length(); j++)
              {
                  if(tempText[j] >= '0' && tempText[j] <= '9')
                      hoursString.append(tempText[j]);
              }
              passCity.append(cityString);
              hours.push_back(hoursString.toInt());
            }
            //qDebug() <<passCity;
            //qDebug() << hours;
        }
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
        //cout << expectedEndTime.day << "天" << expectedEndTime.hour << "时" << expectedEndTime.minute << "分\n";
        //Tourist t(ui->departureBox->currentText().toStdString(), ui->destinationBox->currentText().toStdString(), startTime, expectedEndTime, ui->strategyBox->currentIndex() + 1);
        Tourist t(ui->departureBox->currentText().toStdString(), ui->destinationBox->currentText().toStdString(), passCity, hours,startTime,expectedEndTime, ui->strategyBox->currentIndex() + 1);
        //t.getStrategy();
        t.getPassStrategy();
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
//途经城市列表添加城市
void MainWindow::on_addCity_clicked()
{
    departCity = ui->departureBox->currentText();
    destCity =ui->destinationBox->currentText();
    if(!addedCities.contains(departCity))
        addedCities.append(departCity);
    if(!addedCities.contains(destCity))
        addedCities.append(destCity);
    QString text = "";
    QString city = ui->cityBox->currentText();
    bool found = false;
    for(int i=0; i<addedCities.length(); i++){
        if(addedCities.contains(city)){
            found = true;
            break;
        }
    }
    int hours = ui->visitTime->value();
    if(city.compare("请选择途经城市") && found==false){
        //qDebug() << QString::number(hours);
        addedCities.append(city);
        text=city + '\t' + '\t' + "停留时间：" + QString::number(hours) + "小时";
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(text);
        ui->passList->addItem(item);
        ui->passList->setFocus();
    }
}
//途经城市列表删除城市
void MainWindow::on_deleteCity_clicked()
{
    QListWidgetItem *currentItem = ui->passList->currentItem();
    addedCities.removeAll((*currentItem).text().mid(0,2));
    delete currentItem;
}

void MainWindow::changeDepartCity()
{
    addedCities.removeAll(departCity);
    addedCities.append(ui->departureBox->currentText());
    departCity = ui->departureBox->currentText();
}

void MainWindow::changeDestCity()
{
    addedCities.removeAll(destCity);
    addedCities.append(ui->destinationBox->currentText());
    departCity = ui->destinationBox->currentText();
}
