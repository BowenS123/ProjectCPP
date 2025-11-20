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

    Factory();
    explicit Factory(std::string name);
    Factory(const Factory& other) = delete;
    ~Factory();

    Factory& operator=(const Factory&) = delete;

    // Production & resources
    void produceClanker(Clanker* clankerPtr);
    bool safeProduceWorker();
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

    // Combat
    std::string defendAgainst(Enemy& enemy);

    const std::string& getName() const;
    unsigned char getId() const;
    int getHealth() const;
    bool isDestroyed() const;
    const std::string& getLogPath() const;
    const Clanker* getFirstActiveClanker() const;

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
    unsigned char nextId;
    std::string logPath;
    mutable std::ofstream logFile;
};

}

#endif // FACTORY_H
