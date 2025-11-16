#ifndef FACTORY_H
#define FACTORY_H

#include "clanker.h"
#include "defender.h"
#include "scout.h"
#include "util.h"
#include "worker.h"

#include <vector>
#include <string>
#include <mutex>
#include <thread>
#include <iostream>
#include <fstream>

namespace ClankerSim {

class Enemy;

class FactoryInspector;

class Factory : public Entity {
public:
    static constexpr int MAX_HEALTH = 500;

    Factory();
    explicit Factory(std::string name);
    Factory(const Factory& other);
    ~Factory() override;

    Factory& operator=(const Factory&) = delete;

    void produceClanker(Clanker* clankerPtr);
    bool safeProduceWorker();
    bool produceBattery(int count = 1);

    void updateAll(float dt = 1.0f);
    void update(float dt) override {
        updateAll(dt);
    }

    void takeDamage(int dmg) {
        Entity::takeDamage(dmg); log("Factory damaged" );
    }
    void repair(int hp = 10);

    void addResources(int delta = 0);
    int getResources() const noexcept;
    const std::vector<Clanker*>& getClankers() const noexcept;
    int getBatteries() const noexcept;
    void addBatteries(int diff);

    void startProductionThread();
    void shutdown();

    std::string defendAgainst(Enemy& enemy);

    const std::string& getLogPath() const noexcept {
        return logPath;
    }
    const Clanker* getFirstActiveClanker() const noexcept {
        return firstAlive(clankers);
    }

    friend class FactoryInspector;

private:
    void log(const std::string& message) const;

    std::vector<Clanker*> clankers;
    mutable std::mutex mutex;
    bool running;
    bool defenseAlert;
    bool maintenanceMode;
    bool loggingEnabled;
    int resources;
    int batteryStorage;
    unsigned char nextId;
    std::string logPath;
    mutable std::ofstream logFile;
};

class FactoryInspector {
public:
    static void dump(const Factory& factory);
};

}

#endif // FACTORY_H
