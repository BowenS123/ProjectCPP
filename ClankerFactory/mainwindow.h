#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "factory.h"
#include "worker.h"
#include "scout.h"
#include "defender.h"
#include "enemy.h"

#include <QMainWindow>
#include <QTimer>
#include <future>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow // Vraag 42: Qt class
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Vraag 45: enum class (strongly typed enum)
    enum class LogTone {
        Info,
        Success,
        Warning,
        Danger,
        System,
        Salvage
    };

private slots:
    // Vraag 43: signals/slots (Qt) voor UI-interactie
    void on_pauseButton_clicked();          // Toggle game + spawn timers
    void on_produceButton_clicked();        // Build a clanker of selected type
    void on_produceBatteryButton_clicked(); // Build a battery (resource sink)
    void on_giveBatteryButton_clicked();    // Assign a battery to selected unit
    void on_damageButton_clicked();         // Apply damage to the factory
    void on_restartButton_clicked();        // Reset simulation and UI
    void gameLoop();                        // Main tick: advance sim + combat
    void spawnEnemy();                      // Timer-driven enemy spawns
    void updateBatteryButtonState();        // Enable/disable battery button
    void kickOffSalvageScan();              // Launch background salvage when enemies fall
    void checkSalvageScan();                // Poll the salvage future

private:
    Ui::MainWindow *ui;                        // GUI usage: Generated UI elements owner (Qt Designer integration)
    ClankerSim::Factory factory{"My Factory"}; // Model: owns all clankers
    QTimer* gameTimer;                         // Drives gameLoop each second
    QTimer* enemySpawnTimer;                   // Spawns enemies periodically
    std::vector<ClankerSim::Enemy> enemies;    // Active enemies in the scene. Vraag 36: useful container
    int enemySpawnCount = 0;                   // Counts spawns to scale difficulty
    int lastSelectedId = -1;                   // Remember selection across UI refreshes
    std::future<int> salvageFuture;            // Vraag 41: threads/async for background task
    bool salvageInProgress = false;            // Tracks salvage job state (vraag 31: useful bool)
    QString salvageScoutName;                  // Remembers which scout is out salvaging
    void updateUI();                           // Refresh all widgets from model state
    void appendLog(const QString& text, LogTone tone = LogTone::Info); // Color-coded log writer (vraag 30: const-ref QString parameter)
};

#endif // MAINWINDOW_H
