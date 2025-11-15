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
    ui->defenderEnergyBar->setMinimum(0);
    ui->defenderEnergyBar->setMaximum(100);

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::gameLoop);
    gameTimer->start(1000);

    enemySpawnTimer = new QTimer(this);
    connect(enemySpawnTimer, &QTimer::timeout, this, &MainWindow::spawnEnemy);
    enemySpawnTimer->start(7000);

    updateUI();
}

MainWindow::~MainWindow()
{
    factory.shutdown();
    delete ui;
}

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
    }
    else if (type == "Defender" && factory.getResources() >= 40) {
        factory.addResources(-40);
        auto d = new ClankerSim::DefenderClanker("Defender", 3);
        d->setFactory(&factory);
        factory.produceClanker(d);
        ui->logTextEdit->append("Produced Defender Clanker!");
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

    for (Enemy &e : enemies) {
        if (!e.alive) continue;

        int defenderCount = 0;
        int workerCount = 0;

        for (auto* c : factory.getClankers()) {
            if (dynamic_cast<ClankerSim::DefenderClanker*>(c)) {
                defenderCount++;
                }
            else if (dynamic_cast<ClankerSim::WorkerClanker*>(c)) {
                workerCount++;
            }
        }

        if (defenderCount > 0) {
            e.takeDamage(defenderCount * 10);
            ui->logTextEdit->append("Defenders attack the enemy!");
        }
        else if (workerCount > 0) {
            e.takeDamage(workerCount * 5);
            ui->logTextEdit->append("Workers attack the enemy!");
        }
        else {
            factory.takeDamage(e.attack);
            ui->logTextEdit->append("Enemy hits the factory!");
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy &e){ return !e.alive; }), enemies.end());

    updateUI();
}

void MainWindow::spawnEnemy()
{
    enemySpawnCount++;

    int hp = 40 + enemySpawnCount * 5;
    int attack = 5 + enemySpawnCount / 2;

    enemies.push_back(Enemy(hp, attack));
    ui->logTextEdit->append(QString("An enemy approaches! HP: %1, ATK: %2").arg(hp).arg(attack));

    updateUI();
}

void MainWindow::updateUI()
{
    ui->factoryHealthBar->setValue(factory.health);
    ui->resourcesLabel->setText(QString("Resources: %1").arg(factory.getResources()));
    ui->batteryLabel->setText(QString("Batteries: %1").arg(factory.getBatteries()));
    ui->enemyCountLabel->setText(QString("Enemies: %1").arg(enemies.size()));

    int workerCount = 0, scoutCount = 0, defenderCount = 0;
    int workerEnergy = 0, scoutEnergy = 0, defenderEnergy = 0;

    for (auto* c : factory.getClankers()) {
        if (auto w = dynamic_cast<ClankerSim::WorkerClanker*>(c)) {
            workerCount++;
            workerEnergy += w->getEnergy();
        }
        if (auto s = dynamic_cast<ClankerSim::ScoutClanker*>(c)) {
            scoutCount++;
            scoutEnergy += s->getEnergy();
        }
        if (auto d = dynamic_cast<ClankerSim::DefenderClanker*>(c)) {
            defenderCount++;
            defenderEnergy += d->getEnergy();
        }
    }

    ui->workerCountLabel->setText(QString::number(workerCount));
    ui->scoutCountLabel->setText(QString::number(scoutCount));
    ui->defenderCountLabel->setText(QString::number(defenderCount));

    ui->workerEnergyBar->setMaximum(workerCount * 100);
    ui->workerEnergyBar->setValue(workerEnergy);
    ui->scoutEnergyBar->setMaximum(scoutCount * 100);
    ui->scoutEnergyBar->setValue(scoutEnergy);
    ui->defenderEnergyBar->setMaximum(defenderCount * 100);
    ui->defenderEnergyBar->setValue(defenderEnergy);

    if (factory.isDestroyed()) {
        ui->logTextEdit->append("GAME OVER! Factory destroyed!");
        gameTimer->stop();
        enemySpawnTimer->stop();
    }
}
