#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "factory.h"
#include "worker.h"
#include "scout.h"
#include "defender.h"
#include "enemy.h"

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pauseButton_clicked();          // Toggle game + spawn timers
    void on_produceButton_clicked();        // Build a clanker of selected type
    void on_produceBatteryButton_clicked(); // Build a battery (resource sink)
    void on_giveBatteryButton_clicked();    // Assign a battery to selected unit
    void on_damageButton_clicked();         // Apply damage to the factory
    void on_restartButton_clicked();        // Reset simulation and UI
    void gameLoop();                        // Main tick: advance sim + combat
    void spawnEnemy();                      // Timer-driven enemy spawns
    void updateBatteryButtonState();        // Enable/disable battery button

private:
    Ui::MainWindow *ui;                        // Generated UI elements owner
    ClankerSim::Factory factory{"My Factory"}; // Model: owns all clankers
    QTimer* gameTimer;                         // Drives gameLoop each second
    QTimer* enemySpawnTimer;                   // Spawns enemies periodically
    std::vector<ClankerSim::Enemy> enemies;    // Active enemies in the scene
    int enemySpawnCount = 0;                   // Counts spawns to scale difficulty
    int lastSelectedId = -1;                   // Remember selection across UI refreshes
    void updateUI();                           // Refresh all widgets from model state
};

#endif // MAINWINDOW_H
