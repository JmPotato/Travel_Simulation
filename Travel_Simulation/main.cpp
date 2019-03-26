#include "mainwindow.h"
#include <QApplication>
#include "Tourist.h"

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    Tourist t("北京", "成都", 2);
    t.getStrategy();

    //return a.exec();
    return 0;
}
