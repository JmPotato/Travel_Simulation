#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Strategy.h"
#include "Tourist.h"
#include <QDebug>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QStringList>
#include <queue>

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
    ui->simulatedTime->setDateTime(QDateTime::currentDateTime());
    ui->simulatedProgressBar->setRange(0,1000);
    ui->simulatedProgressBar->setValue(0);
    ui->endTime->setEnabled(false);
    ui->expectedEndTime->setEnabled(false);
    ui->budgetEdit->setEnabled(false);
    ui->pauseButton->setEnabled(false);
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
    planReady = false;
    alreadyStart = false;
    ptimer = new QTimer;

    connect(this->ui->departureBox,SIGNAL(currentTextChanged(QString)),this,SLOT(changeDepartCity()));
    connect(this->ui->destinationBox,SIGNAL(currentTextChanged(QString)),this,SLOT(changeDestCity()));
    connect(ptimer,SIGNAL(timeout()),this,SLOT(changeTravelStatus()));
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
        tourist = t;
        tourist.getPassStrategy();
        ui->logBrowser->setText(tourist.getLog());
        MyTime endTime = startTime + tourist.getPlanResult()->destTime - tourist.getPlanResult()->expectedDepartTime;
        ui->endTime->setDate(ui->startTime->date().addDays(endTime.day));
        ui->endTime->setTime(QTime::fromString("00:00", "hh:mm"));
        ui->endTime->setTime(ui->endTime->time().addSecs(endTime.hour * 3600 + endTime.minute * 60));
        int cost=tourist.getPlanResult()->result.moenyCost;
        ui->budgetEdit->setText(QString("RMB ¥")+QString::number(cost));
        planReady = true;
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
    if (currentItem) {
        addedCities.removeAll((*currentItem).text().mid(0,2));
        delete currentItem;
    }
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

//点击开始模拟旅行
void MainWindow::on_simButton_clicked()
{
    if(planReady == false)
    {
        QMessageBox::information(this,"错误","请先规划旅行线路，再开始模拟旅行","确定");
        return;
    }
    else
    {
        ui->planButton->setEnabled(false);
        ui->simButton->setEnabled(false);
        ui->pauseButton->setEnabled(true);
        ptimer->start(10);                   //可以用来调节模拟进度的快慢
        if(!alreadyStart)                   //还没有开始模拟
        {
            currentMinute =0;
            day = 0;
            pathIndex = 0;
            cityIndex = 0;
            pathes.clear();
            cities.clear();
            MyTime startTime(0, ui->startTime->time().hour(), ui->startTime->time().minute());
            MyTime tempTime(0,0,0);
            MyTime tempTime2(0,0,0);
            Result result = tourist.getPlanResult()->result;
            vector<Path>::iterator iter = result.route.begin();
            cities.append("目前停留在：" + QString::fromStdString((*iter).start));
            for(iter = result.route.begin();iter!= result.route.end();iter++)
            {
                tempTime = (*iter).startTime - startTime;
                int minutes = tempTime.day*24*60+tempTime.hour*60+tempTime.minute;
                pathStartMinutes.enqueue(minutes);
                tempTime2 = (*iter).endTime - startTime;
                //qDebug()<<"path结束时间";
                //tempTime2.print();
                int minutes2 = tempTime2.day*24*60+tempTime2.hour*60+tempTime2.minute;
                //qDebug()<<"path结束分钟"<<minutes2;
                pathEndMinutes.enqueue(minutes2);
                QString startCity  = QString::fromStdString((*iter).start);
                QString endCity  = QString::fromStdString((*iter).end);
                QString currentTool = (*iter).tool;
                QString currentNumber = (*iter).number;
                pathes.append(QString("%1--->%2,%3(%4)").arg(startCity).arg(endCity).arg(currentTool).arg(currentNumber));
                cities.append("目前停留在："+endCity);
                //qDebug()<<"pathStartMinutes:"<<pathStartMinutes;
                //qDebug()<<"pathEndMinutes"<<pathEndMinutes;
            }
            targetMinutes = pathStartMinutes.dequeue();
            targetMinutes2 = pathEndMinutes.dequeue();
            this->ui->statusLabel->setText(QString("%1").arg(cities[cityIndex]));
            //qDebug()<<"pathes:"<<pathes;
            //qDebug()<<"cities:"<<cities;
        }
        alreadyStart = true;
    }
}

void MainWindow::changeTravelStatus()
{
    currentMinute++;
    MyTime startTime(0, ui->startTime->time().hour(), ui->startTime->time().minute());
    MyTime CostTime = tourist.getPlanResult()->result.timeCost;
    int totalMinutes = CostTime.day*24*60+CostTime.hour*60+CostTime.minute;
    int value =1000*currentMinute/totalMinutes;
    ui->simulatedProgressBar->setValue(value);

    QTime ceil(23,59,59,999);
    QTime floor(23,59,0,0);
    if(floor < ui->startTime->time().addSecs(currentMinute*60) && ui->startTime->time().addSecs(currentMinute*60) < ceil)
       day++;
    ui->simulatedTime->setDate(ui->startTime->date().addDays(startTime.day + day));
    ui->simulatedTime->setTime(QTime::fromString("00:00", "hh:mm"));
    ui->simulatedTime->setTime(ui->startTime->time().addSecs(currentMinute*60));

    if(currentMinute == targetMinutes)  //到达新的path
    {
        //qDebug()<<"currentMinute"<<currentMinute;
        pathIndex++;
        //qDebug()<<"path"<<path;
        this->ui->statusLabel->setText(QString("%1").arg(pathes[pathIndex-1]));
        if(pathStartMinutes.size()>0)
            targetMinutes = pathStartMinutes.dequeue();
    }

    if(currentMinute == targetMinutes2)  //到达一个城市
    {
        //qDebug()<<"currentMinute"<<currentMinute;
        cityIndex++;
        //qDebug()<<"city"<<city;
        this->ui->statusLabel->setText(QString("%1").arg(cities[cityIndex]));
        if(pathEndMinutes.size()>0)
            targetMinutes2 = pathEndMinutes.dequeue();
    }

    if(currentMinute == totalMinutes)  //模拟完成
    {
        ptimer->stop();
        alreadyStart = false;
        ui->planButton->setEnabled(true);
        ui->simButton->setEnabled(true);
        ui->simButton->setText("开始模拟");
        ui->pauseButton->setEnabled(false);
        QMessageBox::information(this,"提示","本次模拟结束！","确定");
    }
}

void MainWindow::on_pauseButton_clicked()
{
    ptimer->stop();
    this->ui->pauseButton->setEnabled(false);
    this->ui->simButton->setEnabled(true);
    this->ui->simButton->setText("继续模拟");
}
