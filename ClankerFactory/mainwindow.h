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
    void on_pauseButton_clicked();
    void on_produceButton_clicked();
    void on_produceBatteryButton_clicked();
    void on_giveBatteryButton_clicked();
    void on_damageButton_clicked();
    void gameLoop();
    void spawnEnemy();
    void updateBatteryButtonState();

private:
    Ui::MainWindow *ui;
    ClankerSim::Factory factory{"My Factory"};
    QTimer* gameTimer;
    QTimer* enemySpawnTimer;
    std::vector<ClankerSim::Enemy> enemies;
    int enemySpawnCount = 0;
    int lastSelectedId = -1;

    void updateUI();
};

#endif // MAINWINDOW_H
