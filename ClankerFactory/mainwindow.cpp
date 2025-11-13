#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->factoryHealthBar->setMinimum(0);
    ui->factoryHealthBar->setMaximum(ClankerSim::Factory::MAX_HEALTH);
    ui->factoryHealthBar->setValue(factory.health);

    ui->workerEnergyBar->setMinimum(0);
    ui->workerEnergyBar->setMaximum(100);
    ui->scoutEnergyBar->setMinimum(0);
    ui->scoutEnergyBar->setMaximum(100);

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::gameLoop);

    gameTimer->start(1000);

    updateUI();
}

MainWindow::~MainWindow()
{
    factory.shutdown();
    delete ui;
}

// Pause/Resume button
void MainWindow::on_pauseButton_clicked()
{
    if (gameTimer->isActive()) {
        gameTimer->stop();
        ui->pauseButton->setText("Resume");
        ui->logTextEdit->append("Game paused.");
    } else {
        gameTimer->start(1000);
        ui->pauseButton->setText("Pause");
        ui->logTextEdit->append("Game resumed.");
    }
}

void MainWindow::on_produceButton_clicked()
{
    QString type = ui->clankerTypeComboBox->currentText();

    if (type == "Worker" && factory.getResources() >= 20) {
        factory.addResources(-20);
        auto w = new ClankerSim::WorkerClanker("Worker", 1);
        w->setFactory(&factory);
        factory.produceClanker(w);
        ui->logTextEdit->append("Produced Worker Clanker!");
    }
    else if (type == "Scout" && factory.getResources() >= 30) {
        factory.addResources(-30);
        auto s = new ClankerSim::ScoutClanker("Scout", 2);
        s->setFactory(&factory);
        factory.produceClanker(s);
        ui->logTextEdit->append("Produced Scout Clanker!");
    } else {
        ui->logTextEdit->append("Not enough resources to produce this Clanker!");
    }

    updateUI();
}

void MainWindow::on_produceBatteryButton_clicked()
{
    if (factory.produceBattery()) {
        ui->logTextEdit->append("Produced a battery!");
    } else {
        ui->logTextEdit->append("Not enough resources to produce a battery!");
    }
    updateUI();
}

void MainWindow::on_damageButton_clicked()
{
    factory.takeDamage(50);
    updateUI();
}

void MainWindow::gameLoop()
{
    factory.update(1.0f);
    updateUI();
}

void MainWindow::updateUI()
{
    ui->factoryHealthBar->setValue(factory.health);
    ui->resourcesLabel->setText(QString("Resources: %1").arg(factory.getResources()));
    ui->batteryLabel->setText(QString("Batteries: %1").arg(factory.getBatteries()));

    int workerCount = 0, scoutCount = 0;
    int workerEnergy = 0, scoutEnergy = 0;

    for (auto* c : factory.getClankers()) {
        if (auto w = dynamic_cast<ClankerSim::WorkerClanker*>(c)) {
            workerCount++;
            workerEnergy += w->getEnergy();
        }
        if (auto s = dynamic_cast<ClankerSim::ScoutClanker*>(c)) {
            scoutCount++;
            scoutEnergy += s->getEnergy();
        }
    }

    ui->workerCountLabel->setText(QString::number(workerCount));
    ui->scoutCountLabel->setText(QString::number(scoutCount));

    ui->workerEnergyBar->setMaximum(workerCount * 100);
    ui->workerEnergyBar->setValue(workerEnergy);
    ui->scoutEnergyBar->setMaximum(scoutCount * 100);
    ui->scoutEnergyBar->setValue(scoutEnergy);

    if (factory.isDestroyed()) {
        ui->logTextEdit->append("GAME OVER! Factory destroyed!");
        gameTimer->stop();
    }
}
