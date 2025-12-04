#ifndef CLANKERFACTORY_H
#define CLANKERFACTORY_H

#include <vector>
#include <memory>
#include <string>
#include <mutex>
#include <thread>
#include <iostream>

namespace ClankerSim {

class Entity {
protected:
    std::string name;
    unsigned char id;
    bool active;

public:
    int health;

    Entity(const std::string& n, unsigned char i, int hp) : name(n), id(i), active(true), health(hp) {}
    virtual ~Entity() = default;
    virtual void update(float dt) = 0;

    void takeDamage(int dmg) {
        health -= dmg;
        if (health < 0) {
            health = 0;
        }
    }

    bool isDestroyed() const { return health <= 0; }
    std::string getName() const { return name; }
};

class Factory;

class Clanker : public Entity {
protected:
    int energy;
public:
    Clanker(const std::string& n, unsigned char i, int hp, int en) : Entity(n,i,hp), energy(en) {}

    virtual void work() { energy -= 10; }
    void update(float dt) override { work(); }
    int getEnergy() const { return energy; }
    void recharge(Factory* f);
};

class WorkerClanker : public Clanker {
public:
    WorkerClanker(const std::string& n, unsigned char i) : Clanker(n,i,100,80), factoryRef(nullptr) {}
    void work() override;
    void setFactory(Factory* f) { factoryRef = f; }
private:
    Factory* factoryRef;
};

class ScoutClanker : public Clanker {
public:
    ScoutClanker(const std::string& n, unsigned char i) : Clanker(n,i,80,60), factoryRef(nullptr) {}
    void work() override;
    void setFactory(Factory* f) { factoryRef = f; }
private:
    Factory* factoryRef;
};

class DefenderClanker : public Clanker {
public:
    DefenderClanker(const std::string& n, unsigned char i) : Clanker(n, i, 150, 100), factoryRef(nullptr) {}
    void work() override {
        energy -= 10;
        if (energy <= 20) {
            recharge(factoryRef);
        }
        std::cout << getName() << " attacks enemies!\n";
    }
    void setFactory(Factory* f) { factoryRef = f; }
private:
    Factory* factoryRef;
};

class Factory : public Entity {
private:
    std::vector<std::unique_ptr<Clanker>> clankers;
    std::mutex mtx;
    bool running;
    int resources = 100;
    int batteryStorage = 2;

public:
    static const int MAX_HEALTH = 500;

    Factory(const std::string& n)
        : Entity(n, 255, MAX_HEALTH/2), running(false) {}

    ~Factory() {
        shutdown();
    }

    void produceClanker(std::unique_ptr<Clanker> c) {
        std::lock_guard<std::mutex> lock(mtx);

        Clanker* raw = c.get();
        if (auto worker = dynamic_cast<WorkerClanker*>(raw)) {
            worker->setFactory(this);
        }
        if (auto scout = dynamic_cast<ScoutClanker*>(raw)) {
            scout->setFactory(this);
        }
        if (auto def = dynamic_cast<DefenderClanker*>(raw)) {
            def->setFactory(this);
        }

        std::cout << "Produced clanker: " << raw->getName() << "\n";
        clankers.push_back(std::move(c));
    }

    void updateAll(float dt) {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto& uptr : clankers) {
            Clanker* c = uptr.get();
            if (c && !c->isDestroyed()) {
                c->update(dt);
            }
        }
    }

    void takeDamage(int dmg) {
        Entity::takeDamage(dmg);
        std::cout << "Factory took " << dmg << " damage. HP left: " << health << "\n";
    }

    void repair(int hp) {
        health += hp;
        if (health > MAX_HEALTH) {
            health = MAX_HEALTH;
        }
        std::cout << "Factory repaired! HP: " << health << "\n";
    }

    void addResources(int r) {
        resources += r;
        if (resources < 0) {
            resources = 0;
        }
    }

    int getResources() const { return resources; }
    std::vector<const Clanker*> getClankers() const {
        std::vector<const Clanker*> snapshot;
        snapshot.reserve(clankers.size());
        for (const auto& uptr : clankers) {
            snapshot.push_back(uptr.get());
        }
        return snapshot;
    }

    int getBatteries() const { return batteryStorage; }
    void addBatteries(int n) { batteryStorage += n; if (batteryStorage < 0) batteryStorage = 0; }

    bool produceBattery() {
        const int cost = 15;
        if (resources >= cost) {
            resources -= cost;
            addBatteries(1);
            std::cout << "Produced 1 battery. Total: " << batteryStorage << "\n";
            return true;
        }
        return false;
    }

    void startProductionThread() {
        running = true;
        std::thread([this]() {
            while(running) {
                auto unit = std::make_unique<WorkerClanker>("Worker", 1);
                produceClanker(std::move(unit));
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
        }).detach();
    }

    void shutdown() { running = false; }

    void update(float dt) override { updateAll(dt); }
};

inline void Clanker::recharge(Factory* f) {
    if (f->getBatteries() > 0 && energy < 100) {
        int needed = 100 - energy;
        energy += needed;
        f->addBatteries(-1);
        std::cout << getName() << " recharged using a battery!\n";
    }
}

inline void WorkerClanker::work() {
    if (!factoryRef) return;

    if (energy <= 20) {
        recharge(factoryRef);
        return;
    }

    energy -= 5;

    if (factoryRef->getResources() >= 10 && factoryRef->health < Factory::MAX_HEALTH) {
        factoryRef->addResources(-10);
        factoryRef->repair(15);
        std::cout << getName() << " repairs the factory!\n";
    } else {
        factoryRef->addResources(5);
        std::cout << getName() << " gathers resources.\n";
    }
}


inline void ScoutClanker::work() {
    if (!factoryRef) return;

    if (energy <= 20) {
        recharge(factoryRef);
        return;
    }

    energy -= 5;
    factoryRef->addResources(10);
    std::cout << getName() << " scouts and gathers resources.\n";
}

}

#endif // CLANKERFACTORY_H
