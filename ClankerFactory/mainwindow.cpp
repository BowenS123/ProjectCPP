#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Clanker Factory Command");

    ui->logTextEdit->append("Factory online. Controls ready.");

    ui->clankerTableWidget->setHorizontalHeaderLabels({"Squad", "Active Units", "Avg Energy"});
    ui->clankerTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->clankerTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->clankerTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->clankerTableWidget->verticalHeader()->setVisible(false);

    ui->clankerDetailTableWidget->setHorizontalHeaderLabels({"ID", "Name", "Class", "Energy", "Status"});
    ui->clankerDetailTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->clankerDetailTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->clankerDetailTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->clankerDetailTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->clankerDetailTableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->clankerDetailTableWidget->verticalHeader()->setVisible(false);

    ui->factoryHealthBar->setMinimum(0);
    ui->factoryHealthBar->setMaximum(ClankerSim::Factory::MAX_HEALTH);
    ui->factoryHealthBar->setValue(factory.getHealth());

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
        w->setFactory(factory);
        factory.produceClanker(w);
        ui->logTextEdit->append("Produced Worker Clanker!");
    }
    else if (type == "Scout" && factory.getResources() >= 30) {
        factory.addResources(-30);
        auto s = new ClankerSim::ScoutClanker("Scout", 2);
        s->setFactory(factory);
        factory.produceClanker(s);
        ui->logTextEdit->append("Produced Scout Clanker!");
    }
    else if (type == "Defender" && factory.getResources() >= 40) {
        factory.addResources(-40);
        auto d = new ClankerSim::DefenderClanker("Defender", 3);
        d->setFactory(factory);
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

    for (ClankerSim::Enemy &enemy : enemies) {
        if (!enemy.isAlive()) {
            continue;
        }
        const std::string& outcome = factory.defendAgainst(enemy);
        ui->logTextEdit->append(QString::fromStdString(outcome));
        if (!enemy.isAlive()) {
            ui->logTextEdit->append(QString::fromStdString(enemy.getName() + " defeated!"));
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](ClankerSim::Enemy &enemy){ return !enemy.isAlive(); }), enemies.end());

    updateUI();
}

void MainWindow::spawnEnemy()
{
    enemySpawnCount++;

    int hp = 40 + enemySpawnCount * 5;
    int attack = 5 + enemySpawnCount / 2;

    enemies.emplace_back(hp, attack);
    ui->logTextEdit->append(QString("An enemy approaches! HP: %1, ATK: %2").arg(hp).arg(attack));

    updateUI();
}

void MainWindow::updateUI()
{
    ui->factoryHealthBar->setValue(factory.getHealth());
    ui->resourcesLabel->setText(QString("Resources: %1").arg(factory.getResources()));
    ui->batteryLabel->setText(QString("Batteries: %1").arg(factory.getBatteries()));
    ui->enemyCountLabel->setText(QString("Enemies: %1").arg(static_cast<int>(enemies.size())));

    int workerCount = 0;
    int scoutCount = 0;
    int defenderCount = 0;
    int workerEnergy = 0;
    int scoutEnergy = 0;
    int defenderEnergy = 0;

    struct UnitRow {
        QString name;
        QString type;
        int energy;
        bool destroyed;
        bool inactive;
        unsigned char id;
    };
    const std::vector<ClankerSim::Clanker*>& activeClankers = factory.getClankers();
    std::vector<UnitRow> unitRows;
    unitRows.reserve(activeClankers.size());
    for (auto* c : activeClankers) {
        if (!c) {
            continue;
        }

        const bool destroyed = c->isDestroyed();
        const int energyValue = c->getEnergy();
        const bool inactive = !destroyed && energyValue <= 0;
        QString typeLabel = "Unknown";
        if (auto* w = dynamic_cast<ClankerSim::WorkerClanker*>(c)) {
            typeLabel = "Worker";
            if (!destroyed && !inactive) {
                workerCount++;
                workerEnergy += energyValue;
            }
        } else if (auto* s = dynamic_cast<ClankerSim::ScoutClanker*>(c)) {
            typeLabel = "Scout";
            if (!destroyed && !inactive) {
                scoutCount++;
                scoutEnergy += energyValue;
            }
        } else if (auto* d = dynamic_cast<ClankerSim::DefenderClanker*>(c)) {
            typeLabel = "Defender";
            if (!destroyed && !inactive) {
                defenderCount++;
                defenderEnergy += energyValue;
            }
        }

        unitRows.push_back(UnitRow{
            QString::fromStdString(c->getName()),
            typeLabel,
            energyValue,
            destroyed,
            inactive,
            c->getId()
        });
    }

    ui->clankerTableWidget->clearContents();
    struct SquadRow {
        QString name;
        int count;
        int totalEnergy;
    };
    const SquadRow rows[3] = {
        {"Workers", workerCount, workerEnergy},
        {"Scouts", scoutCount, scoutEnergy},
        {"Defenders", defenderCount, defenderEnergy}
    };
    auto makeItem = [](const QString& text, Qt::Alignment alignment = Qt::AlignCenter) {
        auto* item = new QTableWidgetItem(text);
        item->setTextAlignment(alignment);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        return item;
    };
    for (int row = 0; row < 3; ++row) {
        ui->clankerTableWidget->setItem(row, 0, makeItem(rows[row].name, Qt::AlignLeft | Qt::AlignVCenter));
        ui->clankerTableWidget->setItem(row, 1, makeItem(QString::number(rows[row].count)));
        const int avgEnergy = rows[row].count > 0 ? rows[row].totalEnergy / rows[row].count : 0;
        ui->clankerTableWidget->setItem(row, 2, makeItem(QString("%1%" ).arg(avgEnergy)));
    }

    ui->clankerDetailTableWidget->clearContents();
    ui->clankerDetailTableWidget->setRowCount(static_cast<int>(unitRows.size()));
    for (int row = 0; row < static_cast<int>(unitRows.size()); ++row) {
        const auto& entry = unitRows[row];
        ui->clankerDetailTableWidget->setItem(row, 0, makeItem(QString::number(entry.id)));
        ui->clankerDetailTableWidget->setItem(row, 1, makeItem(entry.name, Qt::AlignLeft | Qt::AlignVCenter));
        ui->clankerDetailTableWidget->setItem(row, 2, makeItem(entry.type));
        ui->clankerDetailTableWidget->setItem(row, 3, makeItem(QString::number(entry.energy)));
        const QString statusText = entry.destroyed ? "Destroyed" : (entry.inactive ? "Inactive" : "Active");
        ui->clankerDetailTableWidget->setItem(row, 4, makeItem(statusText));
    }

    if (enemySpawnCount % 3 == 0) {
        ClankerSim::FactoryInspector::dump(factory);
    }

    if (factory.isDestroyed()) {
        ui->logTextEdit->append("GAME OVER! Factory destroyed!");
        gameTimer->stop();
        enemySpawnTimer->stop();
    }
}
