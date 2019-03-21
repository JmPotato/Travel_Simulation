#include "Strategy.h"
#include <iostream>
#include <set>
using std::set;
#include <QtSql>

Strategy::Strategy(int t, string d1, string d2, Time T)
{
    type = t;
    depart = d1;
    dest = d2;
    expectedTime = T;
}

void Strategy::startStrategy()
{
    switch (type) {
        case 1:
            cheapestStrategy();
            break;
        case 2:
            fastestStrategy();
            break;
        case 3:
            timeLimitStrategy();
            break;
    }
}

void Strategy::cheapestStrategy()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    std::cout << QDir::currentPath().toStdString() << std::endl;
    db.setDatabaseName(QDir::currentPath() + QString("/travel_query.db"));
    if (!db.open())
        std::cerr << "Failed to connect to database" << std::endl;
   // db.exec("select * from time_table");
    QSqlQuery query(db);
    query.exec("select * from time_table");
    //vector<string> cityList;
//    set<string> cityList;
//    string str;
//    //query.first();
//    while (query.next()) {
//        str = query.value("Dep").toString().toStdString();
//        cityList.insert(str);
//    }
//    set<string>::iterator iter;
//    for (iter = cityList.begin(); iter != cityList.end(); iter++)
//        std::cout << *iter << std::endl;
    //std::cout << cityList.end() - cityList.begin() << std::endl;
    std::cout << "旅行你🐎呢，滚" << std::endl;

}

void Strategy::fastestStrategy()
{

}

void Strategy::timeLimitStrategy()
{

}
