#ifndef FACTORY_H
#define FACTORY_H

#include "clanker.h"
#include "defender.h"
#include "scout.h"
#include "worker.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace ClankerSim {

class Enemy;

// Coordinates clankers, resources, and defenses for the UI.
class Factory {
public:
    static constexpr int MAX_HEALTH = 500;
    static constexpr int WORKER_COST = 20;
    static constexpr int SCOUT_COST = 30;
    static constexpr int DEFENDER_COST = 40;

    Factory();
    explicit Factory(std::string name);
    Factory(const Factory& other) = delete;
    ~Factory();

    Factory& operator=(const Factory&) = delete;

    // Reset factory
    void reset(const std::string& nameValue = "Unnamed Factory");

    // Production & resources
    void produceClanker(Clanker* clankerPtr);
    bool produceBattery(int count = 1);

    void updateAll(float dt = 1.0f);
    void update(float dt);

    // Health
    void takeDamage(int dmg);
    void repair(int hp = 10);

    // Resource queries
    void addResources(int delta = 0);
    int getResources() const;
    const std::vector<Clanker*>& getClankers() const;
    int getBatteries() const;
    void addBatteries(int diff);
    bool giveBatteryTo(unsigned char id);

    // Convenience production
    bool produceWorker();
    bool produceScout();
    bool produceDefender();

    // Combat
    std::string defendAgainst(Enemy& enemy);

    const std::string& getName() const;
    unsigned char getId() const;
    int getHealth() const;
    bool isDestroyed() const;

private:
    // Persist a timestamped message so the UI can inspect activity history.
    void log(const std::string& message) const;

    // Identity & overall health
    std::string name;
    unsigned char id;
    int health;

    // Operational state
    std::vector<Clanker*> clankers;
    bool loggingEnabled;
    int resources;
    int batteryStorage;
    std::string logPath;
    mutable std::ofstream logFile;
};

}

#endif // FACTORY_H
