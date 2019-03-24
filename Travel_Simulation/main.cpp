#include "mainwindow.h"
#include "Strategy.h"
#include "Tourist.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Tourist t("深圳", "青岛", 1);
    t.getStrategy();

    return a.exec();
}
