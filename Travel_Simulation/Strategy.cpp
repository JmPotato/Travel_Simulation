#include "Strategy.h"
#include "Graph.h"
#include <iostream>
#include <set>
#include <QtSql>

using std::set;
using std::cout;
using std::endl;

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
    //std::cout << QDir::currentPath().toStdString() << std::endl;
    db.setDatabaseName(QDir::currentPath() + QString("/travel_query.db"));
    if (!db.open())
        std::cerr << "Failed to connect to database" << std::endl;
   // db.exec("select * from time_table");
    QSqlQuery query(db);
    query.exec("select * from time_table");
    vector<string> cityList;
    set<string> _set;
    string str;
    while (query.next()) {
        str = query.value("Dep").toString().toStdString();
        _set.insert(str);
    }
    set<string>::iterator iter;

    cout << "录入城市：" << endl;
    for (iter = _set.begin(); iter != _set.end(); iter++) {
        cityList.push_back(*iter);
        cout << *iter << " ";
    }
    cout << std::endl << cityList.end() - cityList.begin() << endl;

    Graph G(cityList.size());
    G.setVexsList(cityList);

    vector<string>::iterator i, j;
    int value;
    QString select;
    for (i = cityList.begin(); i != cityList.end(); i++)
        for (j = cityList.begin(); j != cityList.end(); j++) {
            if (*i == *j)
                G.setValue(*i, *j, 0);
            else {
                QString s1 = QString::fromStdString(*i);
                QString s2 = QString::fromStdString(*j);
                select = QString("select * from time_table where Dep='%1' and Dest='%2' order by Price asc limit 1").arg(s1).arg(s2);
                query.exec(select);
                query.first();
                value = query.value("Price").toInt();
                G.setValue(*i, *j, value);
            }
        }
     G.printMatrix();
}

void Strategy::fastestStrategy()
{

}

void Strategy::timeLimitStrategy()
{

}
