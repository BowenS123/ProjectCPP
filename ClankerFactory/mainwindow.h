#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "ClankerFactory.h"

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
    void on_damageButton_clicked();
    void gameLoop();

private:
    Ui::MainWindow *ui;
    ClankerSim::Factory factory{"My Factory"};
    QTimer* gameTimer;

    void updateUI();
};

#endif // MAINWINDOW_H
