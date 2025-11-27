#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Clanker Factory Command");

    ui->logTextEdit->append("Factory online. Controls ready.");

    // Squad table gives a quick overview per role.
    ui->clankerTableWidget->setHorizontalHeaderLabels({"Squad", "Active Units", "Avg Energy"});
    ui->clankerTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->clankerTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->clankerTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->clankerTableWidget->verticalHeader()->setVisible(false);

    // Detail table shows every clanker with full status for debugging.
    ui->clankerDetailTableWidget->setColumnCount(7);
    ui->clankerDetailTableWidget->setHorizontalHeaderLabels({"ID", "Name", "Class", "HP", "Energy", "ATK", "Status"});
    ui->clankerDetailTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->clankerDetailTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->clankerDetailTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->clankerDetailTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->clankerDetailTableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->clankerDetailTableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->clankerDetailTableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    ui->clankerDetailTableWidget->verticalHeader()->setVisible(false);

    ui->factoryHealthBar->setMinimum(0);
    ui->factoryHealthBar->setMaximum(ClankerSim::Factory::MAX_HEALTH);
    ui->factoryHealthBar->setValue(factory.getHealth());
    // Initial button state and helpful tooltip.
    ui->giveBatteryButton->setEnabled(false);
    ui->giveBatteryButton->setToolTip("Select a non-destroyed clanker with energy < 100 and ensure batteries > 0");
    ui->clankerDetailTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->clankerDetailTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    // Show production costs in the type selector tooltip for quick reference.
    ui->clankerTypeComboBox->setItemText(0, "Worker");
    ui->clankerTypeComboBox->setItemText(1, "Scout");
    ui->clankerTypeComboBox->setItemText(2, "Defender");
    ui->clankerTypeComboBox->setItemData(0, QVariant(QString("Cost: %1 Resources").arg(ClankerSim::Factory::WORKER_COST)), Qt::ToolTipRole);
    ui->clankerTypeComboBox->setItemData(1, QVariant(QString("Cost: %1 Resources").arg(ClankerSim::Factory::SCOUT_COST)), Qt::ToolTipRole);
    ui->clankerTypeComboBox->setItemData(2, QVariant(QString("Cost: %1 Resources").arg(ClankerSim::Factory::DEFENDER_COST)), Qt::ToolTipRole);

    // Timers drive the game simulation and enemy spawns.
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::gameLoop);
    gameTimer->start(1000);

    enemySpawnTimer = new QTimer(this);
    connect(enemySpawnTimer, &QTimer::timeout, this, &MainWindow::spawnEnemy);
    enemySpawnTimer->start(7000);

    connect(ui->clankerDetailTableWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::updateBatteryButtonState);

    updateUI();
}

MainWindow::~MainWindow()
{
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

    bool ok = false;
    if (type == "Worker") {
        ok = factory.produceWorker();
    } else if (type == "Scout") {
        ok = factory.produceScout();
    } else if (type == "Defender") {
        ok = factory.produceDefender();
    }

    if (ok) {
        ui->logTextEdit->append(QString("Produced %1 Clanker!").arg(type));
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

void MainWindow::on_giveBatteryButton_clicked()
{
    // Use selection from detail table to find clanker ID
    auto* table = ui->clankerDetailTableWidget;
    auto selected = table->selectedItems();
    if (selected.isEmpty()) {
        ui->logTextEdit->append("Select a clanker row first.");
        return;
    }
    // First column holds ID
    int row = selected.first()->row();
    auto* idItem = table->item(row, 0);
    if (!idItem) {
        ui->logTextEdit->append("No ID in selected row.");
        return;
    }
    bool ok = false;
    unsigned char id = static_cast<unsigned char>(idItem->text().toUShort(&ok));
    if (!ok) {
        ui->logTextEdit->append("Invalid ID.");
        return;
    }
    if (factory.getBatteries() <= 0) {
        ui->logTextEdit->append("No batteries available.");
        updateBatteryButtonState();
        return;
    }
    if (factory.giveBatteryTo(id)) {
        ui->logTextEdit->append(QString("Battery given to clanker ID %1").arg(id));
    } else {
        ui->logTextEdit->append("Battery not given (unit may be full or destroyed or missing).");
    }
    updateUI();
    updateBatteryButtonState();
}

void MainWindow::on_damageButton_clicked()
{
    factory.takeDamage(50);
    updateUI();
}

// Main simulation tick: advance clankers and resolve combat.
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

// Spawns progressively tougher enemies on a timer.
void MainWindow::spawnEnemy()
{
    enemySpawnCount++;

    int hp = 40 + enemySpawnCount * 5;
    int attack = 5 + enemySpawnCount / 2;

    enemies.emplace_back(hp, attack);
    ui->logTextEdit->append(QString("An enemy approaches! HP: %1, ATK: %2").arg(hp).arg(attack));

    updateUI();
}

// Push the latest simulation state into every onscreen widget.
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
        int hp;
        int energy;
        int atk;
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
        int attackValue = 0;
        if (auto* w = dynamic_cast<ClankerSim::WorkerClanker*>(c)) {
            typeLabel = "Worker";
            attackValue = ClankerSim::WorkerClanker::RETALIATION_DAMAGE;
            if (!destroyed && !inactive) {
                workerCount++;
                workerEnergy += energyValue;
            }
        } else if (auto* s = dynamic_cast<ClankerSim::ScoutClanker*>(c)) {
            typeLabel = "Scout";
            attackValue = 0;
            if (!destroyed && !inactive) {
                scoutCount++;
                scoutEnergy += energyValue;
            }
        } else if (auto* d = dynamic_cast<ClankerSim::DefenderClanker*>(c)) {
            typeLabel = "Defender";
            attackValue = ClankerSim::DefenderClanker::RETALIATION_DAMAGE;
            if (!destroyed && !inactive) {
                defenderCount++;
                defenderEnergy += energyValue;
            }
        }

        unitRows.push_back(UnitRow{
            QString::fromStdString(c->getName()),
            typeLabel,
            c->getHp(),
            energyValue,
            attackValue,
            destroyed,
            inactive,
            c->getId()
        });
    }

    // Aggregate squad view summarises force readiness.
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

    // Preserve current selection's ID before rebuilding the table.
    if (auto* sel = ui->clankerDetailTableWidget->selectionModel(); sel && sel->hasSelection()) {
        int row = sel->selectedIndexes().isEmpty() ? -1 : sel->selectedIndexes().first().row();
        if (row >= 0) {
            auto* idItem = ui->clankerDetailTableWidget->item(row, 0);
            if (idItem) {
                bool okId = false;
                int idVal = idItem->text().toInt(&okId);
                if (okId) {
                    lastSelectedId = idVal;
                }
            }
        }
    }

    ui->clankerDetailTableWidget->clearContents();
    ui->clankerDetailTableWidget->setRowCount(static_cast<int>(unitRows.size()));
    for (int row = 0; row < static_cast<int>(unitRows.size()); ++row) {
        const auto& entry = unitRows[row];
        ui->clankerDetailTableWidget->setItem(row, 0, makeItem(QString::number(entry.id)));
        ui->clankerDetailTableWidget->setItem(row, 1, makeItem(entry.name, Qt::AlignLeft | Qt::AlignVCenter));
        ui->clankerDetailTableWidget->setItem(row, 2, makeItem(entry.type));
        ui->clankerDetailTableWidget->setItem(row, 3, makeItem(QString::number(entry.hp)));
        ui->clankerDetailTableWidget->setItem(row, 4, makeItem(QString::number(entry.energy)));
        const QString statusText = entry.destroyed ? "Destroyed" : (entry.inactive ? "Inactive" : "Active");
        ui->clankerDetailTableWidget->setItem(row, 5, makeItem(QString::number(entry.atk)));
        ui->clankerDetailTableWidget->setItem(row, 6, makeItem(statusText));
    }

    // Restore selection by ID if possible.
    if (lastSelectedId != -1) {
        for (int row = 0; row < ui->clankerDetailTableWidget->rowCount(); ++row) {
            auto* idItem = ui->clankerDetailTableWidget->item(row, 0);
            if (!idItem) continue;
            bool ok = false;
            int idVal = idItem->text().toInt(&ok);
            if (ok && idVal == lastSelectedId) {
                ui->clankerDetailTableWidget->setCurrentCell(row, 0);
                ui->clankerDetailTableWidget->selectRow(row);
                break;
            }
        }
    }

    updateBatteryButtonState();

    if (factory.isDestroyed()) {
        ui->logTextEdit->append("GAME OVER! Factory destroyed!");
        gameTimer->stop();
        enemySpawnTimer->stop();
    }
}

// Enable the Give Battery button only when selection is valid
void MainWindow::updateBatteryButtonState()
{
    bool enable = false;
    auto* table = ui->clankerDetailTableWidget;
    auto selected = table->selectedItems();
    if (!selected.isEmpty() && factory.getBatteries() > 0) {
        int row = selected.first()->row();
        auto* idItem = table->item(row, 0);
        auto* energyItem = table->item(row, 4);
        auto* statusItem = table->item(row, 6);
        if (idItem && energyItem && statusItem) {
            bool okEnergy = false;
            int energy = energyItem->text().toInt(&okEnergy);
            const QString status = statusItem->text();
            if (okEnergy && status != "Destroyed" && energy < 100) {
                enable = true;
            }
            bool okId = false;
            int idVal = idItem->text().toInt(&okId);
            if (okId) {
                lastSelectedId = idVal;
            }
        }
    }
    ui->giveBatteryButton->setEnabled(enable);
}
