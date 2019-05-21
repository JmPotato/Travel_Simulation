#include "mainwindow.h"

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
    ui->simulatedTime->setEnabled(false);
    ui->changePlanButton->setEnabled(false);
    QStringList strategyList = {"最少费用", "最少用时", "最少费用+时间"};
    ui->strategyBox->addItems(strategyList);
    QStringList addCityList = {"请选择途经城市", "上海", "北京","南京" ,"广州" ,"成都" ,"杭州" ,"武汉" ,"深圳" ,"西安" ,"郑州" ,"重庆" ,"青岛"};
    ui->departureBox->addItems(cityList);
    ui->destinationBox->addItems(cityList);
    ui->cityBox->addItems(addCityList);
    ui->tabWidget->setCurrentIndex(0);
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
    ui->tabWidget->setCurrentIndex(0);
    allPassPoint.clear();
    repaint();
    if (ui->departureBox->currentText() == ui->destinationBox->currentText()){
        ui->logBrowser->setText(QString("您的出发城市和到达城市一样，请重新选择"));
        //qDebug()<<"--点击开始规划，但是出发城市和到达城市一样";
    }
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
       //qDebug()<<"startTime:";
       //startTime.print();
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
      //qDebug()<<"expectedEndTime";
      //expectedEndTime.print();
        //cout << expectedEndTime.day << "天" << expectedEndTime.hour << "时" << expectedEndTime.minute << "分\n";
        //Tourist t(ui->departureBox->currentText().toStdString(), ui->destinationBox->currentText().toStdString(), startTime, expectedEndTime, ui->strategyBox->currentIndex() + 1);

        Tourist t(ui->departureBox->currentText().toStdString(), ui->destinationBox->currentText().toStdString(), passCity, hours,startTime,expectedEndTime, ui->strategyBox->currentIndex() + 1);
        tourist = t;
        tourist.getPassStrategy();
        ui->logBrowser->setText(tourist.getLog());
        //qDebug()<<"--完成旅行方案规划，详细方案信息已输出到logBrowser中，以下为简要信息";
        MyTime endTime = startTime + tourist.getPlanResult()->destTime - tourist.getPlanResult()->expectedDepartTime;
      //qDebug()<<"destTime";
      //tourist.getPlanResult()->destTime.print();
      //qDebug()<<"expectedDepartTime";
      //tourist.getPlanResult()->expectedDepartTime.print();
      //qDebug()<<"endTime";
      //endTime.print();
        ui->endTime->setDate(ui->startTime->date().addDays(endTime.day));
        ui->endTime->setTime(QTime::fromString("00:00", "hh:mm"));
        ui->endTime->setTime(ui->endTime->time().addSecs(endTime.hour * 3600 + endTime.minute * 60));
        int cost=tourist.getPlanResult()->result.moenyCost;
        ui->budgetEdit->setText(QString("RMB ¥")+QString::number(cost));
        planReady = true;
        //qDebug()<<"--出发城市："<<ui->departureBox->currentText();
        //qDebug()<<"--途经城市："<<passCity;
        //qDebug()<<"--终点城市："<<ui->destinationBox->currentText();
        //qDebug()<<"--旅行策略："<<ui->strategyBox->currentText();
        //qDebug()<<"--出发时间："<<ui->startTime->dateTime().toString("yyyy-MM-dd hh:mm");
        //qDebug()<<"--到达时间："<<ui->endTime->dateTime().toString("yyyy-MM-dd hh:mm");
        //qDebug()<<"--总花费金额："<<ui->budgetEdit->text();
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
        //qDebug()<<"--添加中途城市："<<city;
    }
}
//途经城市列表删除城市
void MainWindow::on_deleteCity_clicked()
{
    QListWidgetItem *currentItem = ui->passList->currentItem();
    if (currentItem) {
        //qDebug()<<"--删除中途城市："<<(*currentItem).text().mid(0,2);
        addedCities.removeAll((*currentItem).text().mid(0,2));
        delete currentItem;
    }
}

void MainWindow::changeDepartCity()
{
    addedCities.removeAll(departCity);
    addedCities.append(ui->departureBox->currentText());
    departCity = ui->departureBox->currentText();
    //qDebug()<<"--出发城市改为："<<departCity;
}

void MainWindow::changeDestCity()
{
    addedCities.removeAll(destCity);
    addedCities.append(ui->destinationBox->currentText());
    destCity = ui->destinationBox->currentText();
    //qDebug()<<"--终点城市改为："<<destCity;
}

//点击开始模拟旅行
void MainWindow::on_simButton_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
    if(planReady == false)
    {
        QMessageBox::information(this,"错误","请先规划旅行线路，再开始模拟旅行","确定");
        return;
    }
    else
    {
        ui->departureBox->setEnabled(false);
        ui->destinationBox->setEnabled(false);
        ui->cityBox->setEnabled(false);
        ui->visitTime->setEnabled(false);
        ui->startTime->setEnabled(false);
        ui->planButton->setEnabled(false);
        ui->strategyBox->setEnabled(false);
        ui->simButton->setEnabled(false);
        ui->addCity->setEnabled(false);
        ui->deleteCity->setEnabled(false);
        ui->changePlanButton->setEnabled(false);
        ui->pauseButton->setEnabled(true);
        MyTime CostTime = tourist.getPlanResult()->result.timeCost;
        totalMinutes = CostTime.day*24*60+CostTime.hour*60+CostTime.minute;
        ptimer->start(10);                   //可以用来调节模拟进度的快慢
        if(!alreadyStart)                       //还没有开始模拟
        {
            allPassPoint.clear();
            repaint();
            //qDebug()<<"--开始模拟旅行";
            currentMinute =0;
            day = 0;
            pathIndex = 0;
            cityIndex = 0;
            alreadyPassCity = 0;
            pathes.clear();
            cities.clear();
            MyTime startTime(0, ui->startTime->time().hour(), ui->startTime->time().minute());
            MyTime tempTime(0,0,0);
            MyTime tempTime2(0,0,0);
            Result result = tourist.getPlanResult()->result;
            vector<Path>::iterator iter = result.route.begin();
            cities.append(QString::fromStdString((*iter).start));
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
                cities.append(endCity);
                //qDebug()<<"pathStartMinutes:"<<pathStartMinutes;
                //qDebug()<<"pathEndMinutes"<<pathEndMinutes;
            }
            targetMinutes = pathStartMinutes.dequeue();
            targetMinutes2 = pathEndMinutes.dequeue();
            currentPeriodMinute = targetMinutes2 - targetMinutes;
            lastDepartMinute = targetMinutes;
            this->ui->statusLabel->setText("目前停留在：" + QString("%1").arg(cities[cityIndex]));
            onPath = false;
            //qDebug()<<"pathes:"<<pathes;
            //qDebug()<<"cities:"<<cities;
        }
        else   //已经开始模拟
        {
            //qDebug()<<"--继续模拟旅行";
        }
        alreadyStart = true;
    }
}

void MainWindow::changeTravelStatus()
{
    currentMinute++;
    repaint();
    MyTime startTime(0, ui->startTime->time().hour(), ui->startTime->time().minute());
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
        onPath = true;
        pathIndex++;
        currentPeriodMinute = targetMinutes2 - targetMinutes;
        lastDepartMinute = targetMinutes;
        for (int i = 1; i < ui->passList->count(); i++) {
            QListWidgetItem *tempItem = ui->passList->item(i);
            if (cities[pathIndex] == tempItem->text().mid(0,2)) {
                tempItem->setFlags(Qt::NoItemFlags);
                alreadyPassCity++;
            }
        }
        currentPathStart = cities[cityIndex];
        currentPathEnd = cities[cityIndex+1];
        currentPeriodMinute = targetMinutes2 - targetMinutes;
        lastDepartMinute = targetMinutes;
        this->ui->statusLabel->setText(QString("%1").arg(pathes[pathIndex-1]));
        if(pathStartMinutes.size()>0)
            targetMinutes = pathStartMinutes.dequeue();
    }

    if(currentMinute == targetMinutes2)  //到达一个城市
    {
        onPath = false;
        //qDebug()<<"currentMinute"<<currentMinute;
        cityIndex++;
        //qDebug()<<"city"<<cities;
        currentPeriodMinute = targetMinutes2 - targetMinutes;
        lastDepartMinute = targetMinutes;
        this->ui->statusLabel->setText("目前停留在：" + QString("%1").arg(cities[cityIndex]));
        if(pathEndMinutes.size()>0)
            targetMinutes2 = pathEndMinutes.dequeue();
    }

    if(currentMinute == totalMinutes)  //模拟完成
    {
        ptimer->stop();
        //qDebug()<<"--模拟旅行结束";
        onPath = false;
        alreadyStart = false;
        ui->planButton->setEnabled(true);
        ui->simButton->setEnabled(true);
        ui->simButton->setText("开始模拟");
        ui->pauseButton->setEnabled(false);
        QMessageBox::information(this,"提示","本次模拟结束！","确定");
        ui->departureBox->setEnabled(true);
        ui->destinationBox->setEnabled(true);
        ui->cityBox->setEnabled(true);
        ui->visitTime->setEnabled(true);
        ui->startTime->setEnabled(true);
        ui->planButton->setEnabled(true);
        ui->strategyBox->setEnabled(true);
        ui->simButton->setEnabled(true);
        ui->addCity->setEnabled(true);
        ui->deleteCity->setEnabled(true);
        for (int i = 1; i < ui->passList->count(); i++) {
            QListWidgetItem *tempItem = ui->passList->item(i);
            tempItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        }
    }
}

void MainWindow::on_pauseButton_clicked()
{

    ptimer->stop();
    //qDebug()<<"--暂停模拟旅行";
    this->ui->changePlanButton->setEnabled(true);
    this->ui->pauseButton->setEnabled(false);
    this->ui->simButton->setEnabled(true);
    this->ui->simButton->setText("按原计划继续模拟");
}

//初始化城市绘图坐标点
void MainWindow::initCityPoint() {
    //cityList = {"上海", "北京","南京" ,"广州" ,"成都" ,"杭州" ,"武汉" ,"深圳" ,"西安" ,"郑州" ,"重庆" ,"青岛"};
    //上海
    cityPoint[0].setX(800);
    cityPoint[0].setY(500);
    //北京
    cityPoint[1].setX(500);
    cityPoint[1].setY(100);
    //南京
    cityPoint[2].setX(700);
    cityPoint[2].setY(500);
    //广州
    cityPoint[3].setX(300);
    cityPoint[3].setY(800);
    //成都
    cityPoint[4].setX(200);
    cityPoint[4].setY(500);
    //杭州
    cityPoint[5].setX(750);
    cityPoint[5].setY(600);
    //武汉
    cityPoint[6].setX(300);
    cityPoint[6].setY(500);
    //深圳
    cityPoint[7].setX(350);
    cityPoint[7].setY(800);
    //西安
    cityPoint[8].setX(100);
    cityPoint[8].setY(200);
    //郑州
    cityPoint[9].setX(200);
    cityPoint[9].setY(200);
    //重庆
    cityPoint[10].setX(250);
    cityPoint[10].setY(500);
    //青岛
    cityPoint[11].setX(600);
    cityPoint[11].setY(200);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    if(ui->tabWidget->currentIndex() == 1) {
        Q_UNUSED(event);
        initCityPoint();
        QPainter painter(this);
        QPen pen;
        pen.setWidth(5);
        static double wid = rect().width();
        static double hei = rect().height();
        painter.setRenderHints(QPainter::Antialiasing, true); //抗锯齿
        double new_wid = rect().width()/wid;
        double new_hei = rect().height()/hei;
        double min = qMin(new_wid,new_hei);
        painter.scale(min, min);
        QPointF currentStartPoint = cityPoint[cityList.indexOf(currentPathStart)];
        QPointF currentEndPoint = cityPoint[cityList.indexOf(currentPathEnd)];
        for(int i = 0;i < allPassPoint.count()-1;i++) {
            pen.setColor(Qt::gray);
            painter.setPen(pen);
            painter.drawLine(allPassPoint[i], allPassPoint[i+1]);
        }
        for(int i = 0;i < 12;i++) {
            pen.setColor(Qt::yellow);
            painter.setPen(pen);
            painter.drawPoint(cityPoint[i]);
            pen.setColor(Qt::green);
            painter.setPen(pen);
            painter.drawText(cityPoint[i].x() - 12, cityPoint[i].y() - 10, cityList[i]);
        }
        for(int i = 0;i < allPassPoint.count()-1;i++) {
            pen.setColor(Qt::red);
            painter.setPen(pen);
            painter.drawPoint(allPassPoint[i]);
            painter.drawPoint(allPassPoint[i+1]);
        }
        if(alreadyStart && onPath) {
            pen.setColor(Qt::red);
            painter.setPen(pen);
            painter.drawPoint(currentStartPoint);
            painter.drawPoint(currentEndPoint);
            QPointF currentPoint;
            double xLength = currentEndPoint.x()-currentStartPoint.x();
            double yLength = currentEndPoint.y()-currentStartPoint.y();
            double currentProgress = double(currentMinute - lastDepartMinute) / currentPeriodMinute;
            currentPoint.setX(currentStartPoint.x() + xLength * currentProgress);
            currentPoint.setY(currentStartPoint.y() + yLength * currentProgress);
            pen.setColor(Qt::blue);
            painter.setPen(pen);
            painter.drawLine(currentStartPoint, currentPoint);
            if(allPassPoint.empty())
                allPassPoint.append(currentStartPoint);
            if(currentPoint == currentEndPoint) {
                allPassPoint.append(currentPoint);
            }
        }
    }
}

//模拟中途点击更改计划
void MainWindow::on_changePlanButton_clicked()
{
    if(ui->changePlanButton->text() == "更改计划")
    {
        this->ui->changePlanButton->setText("按修改计划模拟");
        this->ui->simButton->setEnabled(false);
        this->ui->destinationBox->setEnabled(true);
        this->ui->cityBox->setEnabled(true);
        this->ui->visitTime->setEnabled(true);
        this->ui->addCity->setEnabled(true);
        this->ui->deleteCity->setEnabled(true);
        this->ui->strategyBox->setEnabled(true);
    }
    else
    {
        //qDebug()<<"修改计划";
        //获取新的出发时间和出发城市
        QString newDepartCity;
        MyTime newStartTime(0,0,0);
        int newStartMinutes = targetMinutes2 + ui->startTime->time().hour()*60 +ui->startTime->time().minute();
        int bewteenMinutes = targetMinutes2;
        if(onPath == true) //正在某一条path上，以path的终点为起点城市，以到达时间为新期望出发日期
        {
            newDepartCity = cities[cityIndex + 1];
            unsigned short day = newStartMinutes/(24*60);
            unsigned short hour = (newStartMinutes-(day*1440))/60;
            unsigned short minute = newStartMinutes-(day*1440)-(hour*60);
            MyTime _newStartTime(0,hour,minute);
            newStartTime = _newStartTime;
        }
        else               //正停留在城市，以当前城市为起点，以当前时间为为新期望出发日期
        {
            newDepartCity = cities[cityIndex];
            MyTime _newStartTime(0,ui->simulatedTime->time().hour(),ui->simulatedTime->time().minute());
            newStartTime = _newStartTime;
            bewteenMinutes = ui->startTime->dateTime().secsTo(ui->simulatedTime->dateTime())/60;
        }
        int count = ui->passList->count();

        //获取途经城市列表
        QStringList newPassCity;
        vector<int> newHours;
        if(count > 1)
        {
            for(int i=alreadyPassCity+1 ;i < count; i++)
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
              newPassCity.append(cityString);
              newHours.push_back(hoursString.toInt());
            }
        }

        //设置expectedEndTime
        MyTime oldStartTime(0, ui->startTime->time().hour(), ui->startTime->time().minute());
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
        MyTime expectedEndTime = oldStartTime + period;

        //设置新的Tourist，计算新的策略
        Tourist t(newDepartCity.toStdString(), ui->destinationBox->currentText().toStdString(), newPassCity, newHours, newStartTime,expectedEndTime, ui->strategyBox->currentIndex() + 1);
        tourist = t;
        tourist.getPassStrategy();
        ui->logBrowser->setText(tourist.getLog());
        MyTime endTime = newStartTime + tourist.getPlanResult()->destTime - tourist.getPlanResult()->expectedDepartTime;
        ui->endTime->setDate(ui->startTime->date().addDays(endTime.day).addDays(newStartMinutes/(24*60)));
        ui->endTime->setTime(QTime::fromString("00:00", "hh:mm"));
        ui->endTime->setTime(ui->endTime->time().addSecs(endTime.hour * 3600 + endTime.minute * 60));

        //？？？？？钱还没有处理
        int cost=tourist.getPlanResult()->result.moenyCost;
        ui->budgetEdit->setText(QString("RMB ¥")+QString::number(cost));

        //控制进度相关部分
        pathIndex = 0;
        cityIndex = 0;
        pathes.clear();
        cities.clear();
        pathStartMinutes.clear();
        pathEndMinutes.clear();
        MyTime tempTime(0,0,0);
        MyTime tempTime2(0,0,0);
        Result result = tourist.getPlanResult()->result;
        vector<Path>::iterator iter = result.route.begin();
        cities.append(QString::fromStdString((*iter).start));
        for(iter = result.route.begin();iter!= result.route.end();iter++)
        {
            tempTime = (*iter).startTime - newStartTime;
            int minutes = tempTime.day*24*60+tempTime.hour*60+tempTime.minute;
            pathStartMinutes.enqueue(minutes + bewteenMinutes );
            tempTime2 = (*iter).endTime - newStartTime;
            int minutes2 = tempTime2.day*24*60+tempTime2.hour*60+tempTime2.minute;
            pathEndMinutes.enqueue(minutes2 + bewteenMinutes );
            QString startCity  = QString::fromStdString((*iter).start);
            QString endCity  = QString::fromStdString((*iter).end);
            QString currentTool = (*iter).tool;
            QString currentNumber = (*iter).number;
            pathes.append(QString("%1--->%2,%3(%4)").arg(startCity).arg(endCity).arg(currentTool).arg(currentNumber));
            cities.append(endCity);
        }
        targetMinutes = pathStartMinutes.dequeue();
        targetMinutes2 = pathEndMinutes.dequeue();
        totalMinutes = pathEndMinutes.last();
        ptimer->start(10);
    }
}

void MainWindow::on_tabWidget_currentChanged(int index) {
    Q_UNUSED(index);
    repaint();
}
