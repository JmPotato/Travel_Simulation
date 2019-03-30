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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_11;
    QTextBrowser *logBrowser;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_7;
    QLabel *depatureLabel;
    QComboBox *departureBox;
    QHBoxLayout *horizontalLayout_8;
    QLabel *destinationLabel;
    QComboBox *destinationBox;
    QLabel *passLable;
    QListWidget *passList;
    QHBoxLayout *horizontalLayout_6;
    QComboBox *cityBox;
    QPushButton *addCity;
    QPushButton *deleteCity;
    QHBoxLayout *horizontalLayout_2;
    QLabel *startTimeLabel;
    QDateTimeEdit *startTime;
    QHBoxLayout *horizontalLayout_3;
    QLabel *expectedTimeLabel;
    QDateTimeEdit *expectedTime;
    QHBoxLayout *horizontalLayout_4;
    QLabel *endTimeLabel;
    QDateTimeEdit *endTime;
    QHBoxLayout *horizontalLayout;
    QLabel *budgetLabel;
    QLineEdit *budgetEdit;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label;
    QComboBox *strategyBox;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *planButton;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_10;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *simButton;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(664, 445);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_11 = new QHBoxLayout(centralWidget);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        logBrowser = new QTextBrowser(centralWidget);
        logBrowser->setObjectName(QStringLiteral("logBrowser"));

        horizontalLayout_11->addWidget(logBrowser);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        depatureLabel = new QLabel(centralWidget);
        depatureLabel->setObjectName(QStringLiteral("depatureLabel"));

        horizontalLayout_7->addWidget(depatureLabel);

        departureBox = new QComboBox(centralWidget);
        departureBox->setObjectName(QStringLiteral("departureBox"));

        horizontalLayout_7->addWidget(departureBox);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        destinationLabel = new QLabel(centralWidget);
        destinationLabel->setObjectName(QStringLiteral("destinationLabel"));

        horizontalLayout_8->addWidget(destinationLabel);

        destinationBox = new QComboBox(centralWidget);
        destinationBox->setObjectName(QStringLiteral("destinationBox"));

        horizontalLayout_8->addWidget(destinationBox);


        verticalLayout->addLayout(horizontalLayout_8);

        passLable = new QLabel(centralWidget);
        passLable->setObjectName(QStringLiteral("passLable"));

        verticalLayout->addWidget(passLable);

        passList = new QListWidget(centralWidget);
        passList->setObjectName(QStringLiteral("passList"));

        verticalLayout->addWidget(passList);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        cityBox = new QComboBox(centralWidget);
        cityBox->setObjectName(QStringLiteral("cityBox"));

        horizontalLayout_6->addWidget(cityBox);

        addCity = new QPushButton(centralWidget);
        addCity->setObjectName(QStringLiteral("addCity"));

        horizontalLayout_6->addWidget(addCity);

        deleteCity = new QPushButton(centralWidget);
        deleteCity->setObjectName(QStringLiteral("deleteCity"));

        horizontalLayout_6->addWidget(deleteCity);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        startTimeLabel = new QLabel(centralWidget);
        startTimeLabel->setObjectName(QStringLiteral("startTimeLabel"));

        horizontalLayout_2->addWidget(startTimeLabel);

        startTime = new QDateTimeEdit(centralWidget);
        startTime->setObjectName(QStringLiteral("startTime"));

        horizontalLayout_2->addWidget(startTime);

        horizontalLayout_2->setStretch(0, 3);
        horizontalLayout_2->setStretch(1, 7);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        expectedTimeLabel = new QLabel(centralWidget);
        expectedTimeLabel->setObjectName(QStringLiteral("expectedTimeLabel"));

        horizontalLayout_3->addWidget(expectedTimeLabel);

        expectedTime = new QDateTimeEdit(centralWidget);
        expectedTime->setObjectName(QStringLiteral("expectedTime"));

        horizontalLayout_3->addWidget(expectedTime);

        horizontalLayout_3->setStretch(0, 3);
        horizontalLayout_3->setStretch(1, 7);

        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        endTimeLabel = new QLabel(centralWidget);
        endTimeLabel->setObjectName(QStringLiteral("endTimeLabel"));

        horizontalLayout_4->addWidget(endTimeLabel);

        endTime = new QDateTimeEdit(centralWidget);
        endTime->setObjectName(QStringLiteral("endTime"));

        horizontalLayout_4->addWidget(endTime);

        horizontalLayout_4->setStretch(0, 3);
        horizontalLayout_4->setStretch(1, 7);

        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        budgetLabel = new QLabel(centralWidget);
        budgetLabel->setObjectName(QStringLiteral("budgetLabel"));

        horizontalLayout->addWidget(budgetLabel);

        budgetEdit = new QLineEdit(centralWidget);
        budgetEdit->setObjectName(QStringLiteral("budgetEdit"));

        horizontalLayout->addWidget(budgetEdit);

        horizontalLayout->setStretch(0, 3);
        horizontalLayout->setStretch(1, 7);

        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_5->addWidget(label);

        strategyBox = new QComboBox(centralWidget);
        strategyBox->setObjectName(QStringLiteral("strategyBox"));

        horizontalLayout_5->addWidget(strategyBox);

        horizontalLayout_5->setStretch(0, 3);
        horizontalLayout_5->setStretch(1, 7);

        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_2);

        planButton = new QPushButton(centralWidget);
        planButton->setObjectName(QStringLiteral("planButton"));

        horizontalLayout_9->addWidget(planButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_4);

        simButton = new QPushButton(centralWidget);
        simButton->setObjectName(QStringLiteral("simButton"));

        horizontalLayout_10->addWidget(simButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_10);


        horizontalLayout_11->addLayout(verticalLayout);

        horizontalLayout_11->setStretch(0, 8);
        horizontalLayout_11->setStretch(1, 2);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Travel Simulation", nullptr));
        depatureLabel->setText(QApplication::translate("MainWindow", "\345\207\272\345\217\221\345\234\260", nullptr));
        destinationLabel->setText(QApplication::translate("MainWindow", "\347\233\256\347\232\204\345\234\260", nullptr));
        passLable->setText(QApplication::translate("MainWindow", "\351\200\224\345\276\204\345\234\260\345\210\227\350\241\250", nullptr));
        addCity->setText(QApplication::translate("MainWindow", "+", nullptr));
        deleteCity->setText(QApplication::translate("MainWindow", "-", nullptr));
        startTimeLabel->setText(QApplication::translate("MainWindow", "\345\207\272\345\217\221\346\227\245\346\234\237", nullptr));
        expectedTimeLabel->setText(QApplication::translate("MainWindow", "\351\242\204\346\234\237\346\227\266\351\227\264", nullptr));
        endTimeLabel->setText(QApplication::translate("MainWindow", "\345\210\260\350\276\276\346\227\266\351\227\264", nullptr));
        budgetLabel->setText(QApplication::translate("MainWindow", "\351\242\204\347\256\227\351\207\221\351\242\235", nullptr));
        label->setText(QApplication::translate("MainWindow", "\350\247\204\345\210\222\347\255\226\347\225\245", nullptr));
        planButton->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\350\247\204\345\210\222", nullptr));
        simButton->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\346\250\241\346\213\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
