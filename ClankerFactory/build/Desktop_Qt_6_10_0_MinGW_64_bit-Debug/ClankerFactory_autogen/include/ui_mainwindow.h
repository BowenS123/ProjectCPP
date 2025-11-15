/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QPushButton *damageButton;
    QPushButton *pauseButton;
    QPushButton *produceButton;
    QProgressBar *scoutEnergyBar;
    QProgressBar *workerEnergyBar;
    QProgressBar *factoryHealthBar;
    QTextEdit *logTextEdit;
    QLabel *resourcesLabel;
    QLabel *scoutCountLabel;
    QPushButton *produceBatteryButton;
    QLabel *enemyCountLabel;
    QComboBox *clankerTypeComboBox;
    QLabel *workerCountLabel;
    QLabel *batteryLabel;
    QProgressBar *defenderEnergyBar;
    QLabel *defenderCountLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(70, 10, 601, 344));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        damageButton = new QPushButton(layoutWidget);
        damageButton->setObjectName("damageButton");

        gridLayout->addWidget(damageButton, 4, 0, 1, 1);

        pauseButton = new QPushButton(layoutWidget);
        pauseButton->setObjectName("pauseButton");

        gridLayout->addWidget(pauseButton, 1, 0, 1, 1);

        produceButton = new QPushButton(layoutWidget);
        produceButton->setObjectName("produceButton");

        gridLayout->addWidget(produceButton, 2, 0, 1, 1);

        scoutEnergyBar = new QProgressBar(layoutWidget);
        scoutEnergyBar->setObjectName("scoutEnergyBar");
        scoutEnergyBar->setValue(24);

        gridLayout->addWidget(scoutEnergyBar, 3, 3, 1, 1);

        workerEnergyBar = new QProgressBar(layoutWidget);
        workerEnergyBar->setObjectName("workerEnergyBar");
        workerEnergyBar->setValue(24);

        gridLayout->addWidget(workerEnergyBar, 3, 2, 1, 1);

        factoryHealthBar = new QProgressBar(layoutWidget);
        factoryHealthBar->setObjectName("factoryHealthBar");
        factoryHealthBar->setMaximum(500);
        factoryHealthBar->setValue(500);

        gridLayout->addWidget(factoryHealthBar, 0, 0, 1, 1);

        logTextEdit = new QTextEdit(layoutWidget);
        logTextEdit->setObjectName("logTextEdit");

        gridLayout->addWidget(logTextEdit, 5, 0, 1, 1);

        resourcesLabel = new QLabel(layoutWidget);
        resourcesLabel->setObjectName("resourcesLabel");

        gridLayout->addWidget(resourcesLabel, 0, 1, 1, 1);

        scoutCountLabel = new QLabel(layoutWidget);
        scoutCountLabel->setObjectName("scoutCountLabel");

        gridLayout->addWidget(scoutCountLabel, 2, 3, 1, 1);

        produceBatteryButton = new QPushButton(layoutWidget);
        produceBatteryButton->setObjectName("produceBatteryButton");

        gridLayout->addWidget(produceBatteryButton, 3, 0, 1, 1);

        enemyCountLabel = new QLabel(layoutWidget);
        enemyCountLabel->setObjectName("enemyCountLabel");

        gridLayout->addWidget(enemyCountLabel, 4, 2, 1, 1);

        clankerTypeComboBox = new QComboBox(layoutWidget);
        clankerTypeComboBox->addItem(QString());
        clankerTypeComboBox->addItem(QString());
        clankerTypeComboBox->addItem(QString());
        clankerTypeComboBox->setObjectName("clankerTypeComboBox");

        gridLayout->addWidget(clankerTypeComboBox, 2, 1, 1, 1);

        workerCountLabel = new QLabel(layoutWidget);
        workerCountLabel->setObjectName("workerCountLabel");

        gridLayout->addWidget(workerCountLabel, 2, 2, 1, 1);

        batteryLabel = new QLabel(layoutWidget);
        batteryLabel->setObjectName("batteryLabel");

        gridLayout->addWidget(batteryLabel, 0, 2, 1, 1);

        defenderEnergyBar = new QProgressBar(layoutWidget);
        defenderEnergyBar->setObjectName("defenderEnergyBar");
        defenderEnergyBar->setValue(24);

        gridLayout->addWidget(defenderEnergyBar, 3, 4, 1, 1);

        defenderCountLabel = new QLabel(layoutWidget);
        defenderCountLabel->setObjectName("defenderCountLabel");

        gridLayout->addWidget(defenderCountLabel, 2, 4, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        damageButton->setText(QCoreApplication::translate("MainWindow", "Damage", nullptr));
        pauseButton->setText(QCoreApplication::translate("MainWindow", "Pause", nullptr));
        produceButton->setText(QCoreApplication::translate("MainWindow", "Produce Clanker", nullptr));
        resourcesLabel->setText(QCoreApplication::translate("MainWindow", "Resources", nullptr));
        scoutCountLabel->setText(QCoreApplication::translate("MainWindow", "scoutCount", nullptr));
        produceBatteryButton->setText(QCoreApplication::translate("MainWindow", "Produce Battery", nullptr));
        enemyCountLabel->setText(QCoreApplication::translate("MainWindow", "enemyCount", nullptr));
        clankerTypeComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Worker", nullptr));
        clankerTypeComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Scout", nullptr));
        clankerTypeComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Defender", nullptr));

        workerCountLabel->setText(QCoreApplication::translate("MainWindow", "workerCount", nullptr));
        batteryLabel->setText(QCoreApplication::translate("MainWindow", "Battery", nullptr));
        defenderCountLabel->setText(QCoreApplication::translate("MainWindow", "defenderCount", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
