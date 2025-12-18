// Vraag 42: Qt GUI class (QMainWindow) with UI, timers, and logic.
// Vraag 43: signals/slots connections for buttons and timers.
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <algorithm>
#include <chrono>
#include <thread>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Clanker Factory Command");

    appendLog("Factory online. Controls ready.", LogTone::System);

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

    // Factory health bar
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
    // Vraag 32: dynamic allocation (new) for QTimer objects (Qt parent manages cleanup)
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::gameLoop); // Vraag 43: signal/slot
    gameTimer->start(1000); // 1 second per tick

    // Vraag 32: dynamic allocation (new) for another QTimer
    enemySpawnTimer = new QTimer(this);
    connect(enemySpawnTimer, &QTimer::timeout, this, &MainWindow::spawnEnemy); // Vraag 43: signal/slot
    enemySpawnTimer->start(7000); // 7 seconds per spawn

    // Check if "Give Battery" may be used when selection changes.
    connect(ui->clankerDetailTableWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::updateBatteryButtonState); // Vraag 43: signal/slot
    updateUI(); // push initial factory state into all widgets
}

MainWindow::~MainWindow()
{
    // Vraag 33: dynamic memory removing (deleting Qt UI)
    delete ui;
}

void MainWindow::appendLog(const QString& text, LogTone tone)
{
    // Vraag 51: nice extra – color-coded HTML logging with safe escaping
    const auto colorForTone = [](LogTone toneValue) -> QString { // Vraag 40: lambda function (tone -> color)
        switch (toneValue) {
        case LogTone::Success:
            return "#8BC34A";
        case LogTone::Warning:
            return "#FFB74D";
        case LogTone::Danger:
            return "#EF5350";
        case LogTone::System:
            return "#4FC3F7";
        case LogTone::Salvage:
            return "#9575CD";
        case LogTone::Info:
        default:
            return "#ECEFF1";
        }
    };

    const auto weightForTone = [](LogTone toneValue) -> QString { // Vraag 40: lambda function (tone -> weight)
        switch (toneValue) {
        case LogTone::Danger:
        case LogTone::System:
            return "600";
        default:
            return "400";
        }
    };

    const QString escaped = text.toHtmlEscaped(); // escape user text to avoid injecting HTML
    const QString html = QString("<span style=\"color:%1;font-weight:%2;\">%3</span>") .arg(colorForTone(tone), weightForTone(tone), escaped);
    ui->logTextEdit->append(html);
}

void MainWindow::on_pauseButton_clicked()
{
    if (gameTimer->isActive()) {         // Check if timers are currently running
        gameTimer->stop(); // Stop game tick timer
        enemySpawnTimer->stop(); // Stop enemy spawn timer
        ui->pauseButton->setText("Resume"); // Update button text
        appendLog("Game paused.", LogTone::System);
    } else { // Timers are stopped, so resume them
        gameTimer->start(1000); // Restart game tick (1 second)
        enemySpawnTimer->start(7000); // Restart enemy spawn (7 seconds)
        ui->pauseButton->setText("Pause"); // Update button text
        appendLog("Game resumed.", LogTone::System);
    }
}

void MainWindow::on_produceButton_clicked()
{
    QString type = ui->clankerTypeComboBox->currentText(); // Get selected unit type from dropdown

    bool ok = false; // Track if production succeeded
    if (type == "Worker") { // User selected Worker
        ok = factory.produceUnit<ClankerSim::WorkerClanker>(ClankerSim::Factory::WORKER_COST, "Worker"); // Try to build (costs 20 resources)
    } else if (type == "Scout") { // User selected Scout
        ok = factory.produceUnit<ClankerSim::ScoutClanker>(ClankerSim::Factory::SCOUT_COST, "Scout"); // Try to build (costs 30 resources)
    } else if (type == "Defender") { // User selected Defender
        ok = factory.produceUnit<ClankerSim::DefenderClanker>(ClankerSim::Factory::DEFENDER_COST, "Defender"); // Try to build (costs 40 resources)
    }

    if (ok) {                       // Successful production
        appendLog(QString("Produced %1 Clanker!").arg(type), LogTone::Success);
    } else { // Not enough resources
        appendLog("Not enough resources to produce this Clanker!", LogTone::Warning);
    }

    updateUI(); // Refresh display to show new unit and updated resources
}

void MainWindow::on_produceBatteryButton_clicked()
{
    if (factory.produceBattery()) {
        appendLog("Produced a battery!", LogTone::Success);
    } else {
        appendLog("Not enough resources to produce a battery!", LogTone::Warning);
    }
    updateUI();
}

void MainWindow::on_giveBatteryButton_clicked()
{
    // Use selection from detail table to find clanker ID
    auto* table = ui->clankerDetailTableWidget;
    auto selected = table->selectedItems();
    if (selected.isEmpty()) {                                           // No selection made
        appendLog("Select a clanker row first.", LogTone::Warning);
        return;
    }

    if (factory.getBatteries() <= 0) {                                  // No batteries available
        appendLog("No batteries available.", LogTone::Warning);
        updateBatteryButtonState();
        return;
    }

    // Vraag 46: static_cast (safe type conversion)
    unsigned char id = static_cast<unsigned char>(selected[0]->text().toInt());

    if (factory.giveBatteryTo(id)) {
        appendLog(QString("Battery given to clanker ID %1").arg(id), LogTone::Success);
    } else {
        appendLog("Battery not given (unit may be full or destroyed or missing).", LogTone::Warning);
    }
    updateUI();
    updateBatteryButtonState();
}

void MainWindow::on_damageButton_clicked()
{
    factory.takeDamage(50);
    updateUI();
}

void MainWindow::on_restartButton_clicked()
{
    // Stop timers to avoid intermediate updates while resetting
    if (gameTimer->isActive()) gameTimer->stop();
    if (enemySpawnTimer->isActive()) enemySpawnTimer->stop();

    // Reset simulation state
    factory.reset("My Factory");
    enemies.clear();
    enemySpawnCount = 0;
    lastSelectedId = -1;

    // Reset GUI and The Log
    ui->logTextEdit->clear();
    appendLog("Game restarted. Factory online. Controls ready.", LogTone::System);
    ui->pauseButton->setText("Pause");

    updateUI();

    // Restart timers as running
    gameTimer->start(1000);
    enemySpawnTimer->start(7000);
}

// Main simulation tick: advance clankers and resolve combat.
void MainWindow::gameLoop()
{
    factory.updateAll(); // Tell all units to do their work (Worker/Scout/Defender behaviors)
    
    std::ostringstream ss; // String stream for logging factory state
    ss << factory; // Uses operator<< to format factory info
    appendLog("[State] " + QString::fromStdString(ss.str()), LogTone::Info); // Log current state

    for (ClankerSim::Enemy &enemy : enemies) { // Process each enemy that's currently attacking
        if (!enemy.isAlive()) { // Skip dead enemies
            continue;
        }
        const std::string& outcome = factory.defendAgainst(enemy); // Run combat - defenders/workers fight back
        appendLog(QString::fromStdString(outcome), LogTone::Danger); // Log combat results
        if (!enemy.isAlive()) { // Check if we killed the enemy
            appendLog(QString::fromStdString(enemy.getName() + " defeated!"), LogTone::Success);
            kickOffSalvageScan(); // Start background thread to salvage dead enemy for resources
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](ClankerSim::Enemy &enemy){ return !enemy.isAlive(); }), enemies.end()); // Remove dead enemies from list

    updateUI(); // Refresh display with new stats
    checkSalvageScan(); // Check if background salvage operation finished
}

// Spawns progressively tougher enemies on a timer.
void MainWindow::spawnEnemy()
{
    enemySpawnCount++; // Track how many waves we've spawned (for difficulty scaling)

    int hp = 40 + enemySpawnCount * 5; // Each wave: enemies get +5 HP
    int attack = 5 + enemySpawnCount / 2;

    enemies.emplace_back(hp, attack);
    appendLog(QString("An enemy approaches! HP: %1, ATK: %2").arg(hp).arg(attack), LogTone::Danger);

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
    const std::vector<const ClankerSim::Clanker*>& activeClankers = factory.getClankers();
    std::vector<UnitRow> unitRows;
    unitRows.reserve(activeClankers.size());
    for (auto* c : activeClankers) {
        if (!c) {
            continue;
        }

        // Vraag 31: useful bool (isDestroyed(), inactive check)
        const bool destroyed = c->isDestroyed();
        const int energyValue = c->getEnergy();
        // Vraag 31: useful bool (inactive check)
        const bool inactive = !destroyed && energyValue <= 0;
        QString typeLabel = "Unknown";
        int attackValue = 0;
        if (auto* w = dynamic_cast<const ClankerSim::WorkerClanker*>(c)) {
            typeLabel = "Worker";
            attackValue = ClankerSim::WorkerClanker::RETALIATION_DAMAGE;
            if (!destroyed && !inactive) {
                workerCount++;
                workerEnergy += energyValue;
            }
        } else if (auto* s = dynamic_cast<const ClankerSim::ScoutClanker*>(c)) {
            typeLabel = "Scout";
            attackValue = 0;
            if (!destroyed && !inactive) {
                scoutCount++;
                scoutEnergy += energyValue;
            }
        } else if (auto* d = dynamic_cast<const ClankerSim::DefenderClanker*>(c)) {
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
        
    auto makeItem = [](const QString& text, Qt::Alignment alignment = Qt::AlignCenter) { // Vraag 40: lambda function (small factory for QTableWidgetItem)
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
        appendLog("GAME OVER! Factory destroyed!", LogTone::Danger);
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

void MainWindow::kickOffSalvageScan()
{
    if (salvageInProgress)
        return;

    const auto clankers = factory.getClankers();
    const ClankerSim::ScoutClanker* availableScout = nullptr;
    for (const auto* unit : clankers) {
        if (!unit || unit->isDestroyed()) {
            continue;
        }
        if (unit->getEnergy() <= 20) {
            continue;
        }
        if (const auto* scout = dynamic_cast<const ClankerSim::ScoutClanker*>(unit)) {
            availableScout = scout;
            break;
        }
    }

    if (!availableScout) {
        appendLog("[Salvage] Need a ready scout to clean up.", LogTone::Warning);
        return;
    }

    salvageInProgress = true;
    salvageScoutName = QString::fromStdString(availableScout->getName());
    appendLog(QString("[Salvage] %1 moves to clean the wreck.").arg(salvageScoutName), LogTone::Salvage);

    const int resourcesSnapshot = factory.getResources();
    const int nearbyThreats = static_cast<int>(enemies.size());

    // Vraag 41: usage of threads (std::async for salvage calculation)
    salvageFuture = std::async(std::launch::async, [resourcesSnapshot, nearbyThreats]() { // Vraag 49: external library (C++ std::async)
        std::this_thread::sleep_for(std::chrono::milliseconds(700));
        int total = 6 + nearbyThreats * 2 + std::max(0, 150 - resourcesSnapshot) / 30;
        if (total < 5) total = 5;
        if (total > 18) total = 18;
        return total;
    });
}

void MainWindow::checkSalvageScan()
{
    if (!salvageInProgress)
        return;

    if (!salvageFuture.valid()) {
        salvageInProgress = false;
        return;
    }

    if (salvageFuture.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        return;
    }
    const int bonus = salvageFuture.get();
    salvageInProgress = false;
    factory.addResources(bonus);
    const QString scoutLabel = salvageScoutName.isEmpty() ? QStringLiteral("Scout") : salvageScoutName;
    appendLog(QString("[Salvage] %1 hauled +%2 resources.").arg(scoutLabel).arg(bonus), LogTone::Salvage);
    salvageScoutName.clear();
    updateUI();
}
