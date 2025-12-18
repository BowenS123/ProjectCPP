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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *centralLayout;
    QHBoxLayout *statusRowLayout;
    QGroupBox *factoryGroupBox;
    QVBoxLayout *factoryStatusLayout;
    QHBoxLayout *factoryHealthLayout;
    QLabel *factoryHealthLabel;
    QProgressBar *factoryHealthBar;
    QLabel *resourcesLabel;
    QLabel *batteryLabel;
    QGroupBox *enemyGroupBox;
    QVBoxLayout *enemyLayout;
    QLabel *enemyInfoLabel;
    QLabel *enemyCountLabel;
    QPushButton *damageButton;
    QGroupBox *clankerGroupBox;
    QVBoxLayout *clankerTableLayout;
    QLabel *clankerTableHintLabel;
    QTableWidget *clankerTableWidget;
    QLabel *unitTableHintLabel;
    QTableWidget *clankerDetailTableWidget;
    QHBoxLayout *controlsAndLogLayout;
    QGroupBox *controlsGroupBox;
    QGridLayout *controlsGridLayout;
    QLabel *clankerTypeLabel;
    QComboBox *clankerTypeComboBox;
    QPushButton *produceButton;
    QPushButton *produceBatteryButton;
    QPushButton *giveBatteryButton;
    QPushButton *pauseButton;
    QPushButton *restartButton;
    QGroupBox *logGroupBox;
    QVBoxLayout *logLayout;
    QLabel *logHintLabel;
    QTextEdit *logTextEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralLayout = new QVBoxLayout(centralwidget);
        centralLayout->setObjectName("centralLayout");
        statusRowLayout = new QHBoxLayout();
        statusRowLayout->setObjectName("statusRowLayout");
        factoryGroupBox = new QGroupBox(centralwidget);
        factoryGroupBox->setObjectName("factoryGroupBox");
        factoryStatusLayout = new QVBoxLayout(factoryGroupBox);
        factoryStatusLayout->setObjectName("factoryStatusLayout");
        factoryHealthLayout = new QHBoxLayout();
        factoryHealthLayout->setObjectName("factoryHealthLayout");
        factoryHealthLabel = new QLabel(factoryGroupBox);
        factoryHealthLabel->setObjectName("factoryHealthLabel");

        factoryHealthLayout->addWidget(factoryHealthLabel);

        factoryHealthBar = new QProgressBar(factoryGroupBox);
        factoryHealthBar->setObjectName("factoryHealthBar");
        factoryHealthBar->setMaximum(500);
        factoryHealthBar->setValue(500);

        factoryHealthLayout->addWidget(factoryHealthBar);


        factoryStatusLayout->addLayout(factoryHealthLayout);

        resourcesLabel = new QLabel(factoryGroupBox);
        resourcesLabel->setObjectName("resourcesLabel");

        factoryStatusLayout->addWidget(resourcesLabel);

        batteryLabel = new QLabel(factoryGroupBox);
        batteryLabel->setObjectName("batteryLabel");

        factoryStatusLayout->addWidget(batteryLabel);


        statusRowLayout->addWidget(factoryGroupBox);

        enemyGroupBox = new QGroupBox(centralwidget);
        enemyGroupBox->setObjectName("enemyGroupBox");
        enemyLayout = new QVBoxLayout(enemyGroupBox);
        enemyLayout->setObjectName("enemyLayout");
        enemyInfoLabel = new QLabel(enemyGroupBox);
        enemyInfoLabel->setObjectName("enemyInfoLabel");
        enemyInfoLabel->setAlignment(Qt::AlignCenter);

        enemyLayout->addWidget(enemyInfoLabel);

        enemyCountLabel = new QLabel(enemyGroupBox);
        enemyCountLabel->setObjectName("enemyCountLabel");
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        enemyCountLabel->setFont(font);
        enemyCountLabel->setAlignment(Qt::AlignCenter);

        enemyLayout->addWidget(enemyCountLabel);

        damageButton = new QPushButton(enemyGroupBox);
        damageButton->setObjectName("damageButton");

        enemyLayout->addWidget(damageButton);


        statusRowLayout->addWidget(enemyGroupBox);


        centralLayout->addLayout(statusRowLayout);

        clankerGroupBox = new QGroupBox(centralwidget);
        clankerGroupBox->setObjectName("clankerGroupBox");
        clankerTableLayout = new QVBoxLayout(clankerGroupBox);
        clankerTableLayout->setObjectName("clankerTableLayout");
        clankerTableHintLabel = new QLabel(clankerGroupBox);
        clankerTableHintLabel->setObjectName("clankerTableHintLabel");

        clankerTableLayout->addWidget(clankerTableHintLabel);

        clankerTableWidget = new QTableWidget(clankerGroupBox);
        if (clankerTableWidget->columnCount() < 3)
            clankerTableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        clankerTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        clankerTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        clankerTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (clankerTableWidget->rowCount() < 3)
            clankerTableWidget->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        clankerTableWidget->setVerticalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        clankerTableWidget->setVerticalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        clankerTableWidget->setVerticalHeaderItem(2, __qtablewidgetitem5);
        clankerTableWidget->setObjectName("clankerTableWidget");
        clankerTableWidget->setAlternatingRowColors(true);
        clankerTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        clankerTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        clankerTableWidget->setShowGrid(false);
        clankerTableWidget->setFocusPolicy(Qt::NoFocus);
        clankerTableWidget->setRowCount(3);
        clankerTableWidget->setColumnCount(3);

        clankerTableLayout->addWidget(clankerTableWidget);

        unitTableHintLabel = new QLabel(clankerGroupBox);
        unitTableHintLabel->setObjectName("unitTableHintLabel");

        clankerTableLayout->addWidget(unitTableHintLabel);

        clankerDetailTableWidget = new QTableWidget(clankerGroupBox);
        if (clankerDetailTableWidget->columnCount() < 5)
            clankerDetailTableWidget->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        clankerDetailTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        clankerDetailTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        clankerDetailTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        clankerDetailTableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        clankerDetailTableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem10);
        clankerDetailTableWidget->setObjectName("clankerDetailTableWidget");
        clankerDetailTableWidget->setAlternatingRowColors(true);
        clankerDetailTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        clankerDetailTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        clankerDetailTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        clankerDetailTableWidget->setShowGrid(false);
        clankerDetailTableWidget->setFocusPolicy(Qt::NoFocus);
        clankerDetailTableWidget->setRowCount(0);
        clankerDetailTableWidget->setColumnCount(5);

        clankerTableLayout->addWidget(clankerDetailTableWidget);


        centralLayout->addWidget(clankerGroupBox);

        controlsAndLogLayout = new QHBoxLayout();
        controlsAndLogLayout->setObjectName("controlsAndLogLayout");
        controlsGroupBox = new QGroupBox(centralwidget);
        controlsGroupBox->setObjectName("controlsGroupBox");
        controlsGridLayout = new QGridLayout(controlsGroupBox);
        controlsGridLayout->setObjectName("controlsGridLayout");
        clankerTypeLabel = new QLabel(controlsGroupBox);
        clankerTypeLabel->setObjectName("clankerTypeLabel");

        controlsGridLayout->addWidget(clankerTypeLabel, 0, 0, 1, 1);

        clankerTypeComboBox = new QComboBox(controlsGroupBox);
        clankerTypeComboBox->addItem(QString());
        clankerTypeComboBox->addItem(QString());
        clankerTypeComboBox->addItem(QString());
        clankerTypeComboBox->setObjectName("clankerTypeComboBox");

        controlsGridLayout->addWidget(clankerTypeComboBox, 1, 0, 1, 1);

        produceButton = new QPushButton(controlsGroupBox);
        produceButton->setObjectName("produceButton");

        controlsGridLayout->addWidget(produceButton, 2, 0, 1, 1);

        produceBatteryButton = new QPushButton(controlsGroupBox);
        produceBatteryButton->setObjectName("produceBatteryButton");

        controlsGridLayout->addWidget(produceBatteryButton, 3, 0, 1, 1);

        giveBatteryButton = new QPushButton(controlsGroupBox);
        giveBatteryButton->setObjectName("giveBatteryButton");

        controlsGridLayout->addWidget(giveBatteryButton, 4, 0, 1, 1);

        pauseButton = new QPushButton(controlsGroupBox);
        pauseButton->setObjectName("pauseButton");

        controlsGridLayout->addWidget(pauseButton, 5, 0, 1, 1);

        restartButton = new QPushButton(controlsGroupBox);
        restartButton->setObjectName("restartButton");

        controlsGridLayout->addWidget(restartButton, 6, 0, 1, 1);


        controlsAndLogLayout->addWidget(controlsGroupBox);

        logGroupBox = new QGroupBox(centralwidget);
        logGroupBox->setObjectName("logGroupBox");
        logLayout = new QVBoxLayout(logGroupBox);
        logLayout->setObjectName("logLayout");
        logHintLabel = new QLabel(logGroupBox);
        logHintLabel->setObjectName("logHintLabel");

        logLayout->addWidget(logHintLabel);

        logTextEdit = new QTextEdit(logGroupBox);
        logTextEdit->setObjectName("logTextEdit");
        logTextEdit->setReadOnly(true);

        logLayout->addWidget(logTextEdit);


        controlsAndLogLayout->addWidget(logGroupBox);


        centralLayout->addLayout(controlsAndLogLayout);

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
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Clanker Factory Command", nullptr));
        factoryGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Factory Status", nullptr));
        factoryHealthLabel->setText(QCoreApplication::translate("MainWindow", "Health", nullptr));
        resourcesLabel->setText(QCoreApplication::translate("MainWindow", "Resources: 0", nullptr));
        batteryLabel->setText(QCoreApplication::translate("MainWindow", "Batteries: 0", nullptr));
        enemyGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Threat Monitor", nullptr));
        enemyInfoLabel->setText(QCoreApplication::translate("MainWindow", "Spotted enemies", nullptr));
        enemyCountLabel->setText(QCoreApplication::translate("MainWindow", "Enemies: 0", nullptr));
        damageButton->setText(QCoreApplication::translate("MainWindow", "Simulate Incoming Damage", nullptr));
        clankerGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Clanker Squads", nullptr));
        clankerTableHintLabel->setText(QCoreApplication::translate("MainWindow", "Counts update live as you build or lose units.", nullptr));
        QTableWidgetItem *___qtablewidgetitem = clankerTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Squad", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = clankerTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Active Units", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = clankerTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Avg Energy", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = clankerTableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Workers", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = clankerTableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Scouts", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = clankerTableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Defenders", nullptr));
        unitTableHintLabel->setText(QCoreApplication::translate("MainWindow", "Individual squads appear below with live energy and status.", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = clankerDetailTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = clankerDetailTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = clankerDetailTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Class", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = clankerDetailTableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "Energy", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = clankerDetailTableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        controlsGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Factory Controls", nullptr));
        clankerTypeLabel->setText(QCoreApplication::translate("MainWindow", "Choose clanker type", nullptr));
        clankerTypeComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Worker", nullptr));
        clankerTypeComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Scout", nullptr));
        clankerTypeComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Defender", nullptr));

        produceButton->setText(QCoreApplication::translate("MainWindow", "Produce Selected Clanker", nullptr));
        produceBatteryButton->setText(QCoreApplication::translate("MainWindow", "Produce Battery", nullptr));
        giveBatteryButton->setText(QCoreApplication::translate("MainWindow", "Give Battery to Selected", nullptr));
        pauseButton->setText(QCoreApplication::translate("MainWindow", "Pause", nullptr));
        restartButton->setText(QCoreApplication::translate("MainWindow", "Restart", nullptr));
        logGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Mission Log", nullptr));
        logHintLabel->setText(QCoreApplication::translate("MainWindow", "Latest events", nullptr));
        logTextEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Production updates, combat results, and warnings appear here.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
