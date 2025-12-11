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
#include <memory>
#include <stdexcept>
#include <sstream>

namespace ClankerSim {

class Enemy;
class Factory;

// Friend function for logging factory state
std::ostream& operator<<(std::ostream& os, const Factory& f);

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
    void reset(const std::string& nameValue = "Unnamed Factory");
    void produceClanker(std::unique_ptr<Clanker> clankerPtr);
    bool produceBattery();
    void updateAll(float dt = 1.0f);
    void takeDamage(int dmg);
    void repair(int hp = 10);
    void addResources(int delta = 0);
    int getResources() const;
    std::vector<const Clanker*> getClankers() const;
    int getBatteries() const;
    void addBatteries(int diff);
    bool giveBatteryTo(unsigned char id);
    
    // Template function for flexible unit production
    template<typename UnitType>
    bool produceUnit(int cost, const std::string& unitName = "Unit") {
        if (resources < cost) {
            return false;
        }
        resources -= cost;
        auto unit = std::make_unique<UnitType>(unitName, 0);
        unsigned char id = unit->getId();
        produceClanker(std::move(unit));
        log(std::string("Produced ") + unitName + " (ID=" + std::to_string(id) + ")");
        return true;
    }
    
    // bool produceWorker();
    // bool produceScout();
    // bool produceDefender();

    std::string defendAgainst(Enemy& enemy);
    const std::string& getName() const;
    unsigned char getId() const;
    int getHealth() const;
    bool isDestroyed() const;
    
    friend std::ostream& operator<<(std::ostream& os, const Factory& f);

private:
    void log(const std::string& message) const;

    std::string name;
    unsigned char id;
    int health;
    std::vector<std::unique_ptr<Clanker>> clankers;
    
    bool loggingEnabled;
    int resources;
    int batteryStorage;
    std::string logPath;
    mutable std::ofstream logFile;
};

}  // namespace ClankerSim

#endif // FACTORY_H
