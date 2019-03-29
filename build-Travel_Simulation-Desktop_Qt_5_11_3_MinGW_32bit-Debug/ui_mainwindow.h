/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTextBrowser *logBrowser;
    QGridLayout *gridLayout;
    QPushButton *simButton;
    QListWidget *passList;
    QLineEdit *budgetEdit;
    QLabel *passLable;
    QDateTimeEdit *endTime;
    QLabel *destinationLabel;
    QPushButton *planButton;
    QDateTimeEdit *startTime;
    QComboBox *departureBox;
    QLabel *budgetLabel;
    QPushButton *addCity;
    QPushButton *deleteCity;
    QLabel *endTimeLabel;
    QLabel *depatureLabel;
    QComboBox *destinationBox;
    QComboBox *cityBox;
    QLabel *startTimeLabel;
    QLabel *label;
    QComboBox *strategyBox;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(664, 445);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        logBrowser = new QTextBrowser(centralWidget);
        logBrowser->setObjectName(QStringLiteral("logBrowser"));

        horizontalLayout->addWidget(logBrowser);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        simButton = new QPushButton(centralWidget);
        simButton->setObjectName(QStringLiteral("simButton"));

        gridLayout->addWidget(simButton, 16, 0, 1, 4);

        passList = new QListWidget(centralWidget);
        passList->setObjectName(QStringLiteral("passList"));

        gridLayout->addWidget(passList, 4, 0, 1, 4);

        budgetEdit = new QLineEdit(centralWidget);
        budgetEdit->setObjectName(QStringLiteral("budgetEdit"));

        gridLayout->addWidget(budgetEdit, 13, 1, 1, 3);

        passLable = new QLabel(centralWidget);
        passLable->setObjectName(QStringLiteral("passLable"));

        gridLayout->addWidget(passLable, 3, 0, 1, 1);

        endTime = new QDateTimeEdit(centralWidget);
        endTime->setObjectName(QStringLiteral("endTime"));

        gridLayout->addWidget(endTime, 12, 1, 1, 3);

        destinationLabel = new QLabel(centralWidget);
        destinationLabel->setObjectName(QStringLiteral("destinationLabel"));

        gridLayout->addWidget(destinationLabel, 2, 0, 1, 1);

        planButton = new QPushButton(centralWidget);
        planButton->setObjectName(QStringLiteral("planButton"));

        gridLayout->addWidget(planButton, 15, 0, 1, 4);

        startTime = new QDateTimeEdit(centralWidget);
        startTime->setObjectName(QStringLiteral("startTime"));

        gridLayout->addWidget(startTime, 11, 1, 1, 3);

        departureBox = new QComboBox(centralWidget);
        departureBox->setObjectName(QStringLiteral("departureBox"));

        gridLayout->addWidget(departureBox, 0, 1, 1, 3);

        budgetLabel = new QLabel(centralWidget);
        budgetLabel->setObjectName(QStringLiteral("budgetLabel"));

        gridLayout->addWidget(budgetLabel, 13, 0, 1, 1);

        addCity = new QPushButton(centralWidget);
        addCity->setObjectName(QStringLiteral("addCity"));

        gridLayout->addWidget(addCity, 5, 2, 1, 1);

        deleteCity = new QPushButton(centralWidget);
        deleteCity->setObjectName(QStringLiteral("deleteCity"));

        gridLayout->addWidget(deleteCity, 5, 3, 1, 1);

        endTimeLabel = new QLabel(centralWidget);
        endTimeLabel->setObjectName(QStringLiteral("endTimeLabel"));

        gridLayout->addWidget(endTimeLabel, 12, 0, 1, 1);

        depatureLabel = new QLabel(centralWidget);
        depatureLabel->setObjectName(QStringLiteral("depatureLabel"));

        gridLayout->addWidget(depatureLabel, 0, 0, 1, 1);

        destinationBox = new QComboBox(centralWidget);
        destinationBox->setObjectName(QStringLiteral("destinationBox"));

        gridLayout->addWidget(destinationBox, 2, 1, 1, 3);

        cityBox = new QComboBox(centralWidget);
        cityBox->setObjectName(QStringLiteral("cityBox"));

        gridLayout->addWidget(cityBox, 5, 0, 1, 2);

        startTimeLabel = new QLabel(centralWidget);
        startTimeLabel->setObjectName(QStringLiteral("startTimeLabel"));

        gridLayout->addWidget(startTimeLabel, 11, 0, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 14, 0, 1, 1);

        strategyBox = new QComboBox(centralWidget);
        strategyBox->setObjectName(QStringLiteral("strategyBox"));

        gridLayout->addWidget(strategyBox, 14, 1, 1, 3);


        horizontalLayout->addLayout(gridLayout);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Travel Simulation", nullptr));
        simButton->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\346\250\241\346\213\237", nullptr));
        passLable->setText(QApplication::translate("MainWindow", "\351\200\224\345\276\204\345\234\260\345\210\227\350\241\250", nullptr));
        destinationLabel->setText(QApplication::translate("MainWindow", "\347\233\256\347\232\204\345\234\260", nullptr));
        planButton->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\350\247\204\345\210\222", nullptr));
        budgetLabel->setText(QApplication::translate("MainWindow", "\351\242\204\347\256\227\351\207\221\351\242\235", nullptr));
        addCity->setText(QApplication::translate("MainWindow", "+", nullptr));
        deleteCity->setText(QApplication::translate("MainWindow", "-", nullptr));
        endTimeLabel->setText(QApplication::translate("MainWindow", "\345\210\260\350\276\276\346\227\266\351\227\264", nullptr));
        depatureLabel->setText(QApplication::translate("MainWindow", "\345\207\272\345\217\221\345\234\260", nullptr));
        startTimeLabel->setText(QApplication::translate("MainWindow", "\345\207\272\345\217\221\346\227\245\346\234\237", nullptr));
        label->setText(QApplication::translate("MainWindow", "\350\247\204\345\210\222\347\255\226\347\225\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
