#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDate>
#include <QDateTime>
#include <QTextStream>
#include <QMainWindow>
#include <QSet>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_planButton_clicked();

    void on_strategyBox_currentIndexChanged(int index);

    void on_addCity_clicked();

    void on_deleteCity_clicked();
    void changeDepartCity();
    void changeDestCity();

private:
    Ui::MainWindow *ui;
    QList<QString> addedCities;
    QString departCity;
    QString destCity;
};

#endif // MAINWINDOW_H
