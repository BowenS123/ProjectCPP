#include "factory.h"
#include "enemy.h"

namespace ClankerSim {

Factory::Factory() : Factory("Unnamed Factory") {}

Factory::Factory(std::string name)
    : Entity(std::move(name), 255, MAX_HEALTH / 2),
      running(false),
      defenseAlert(false),
      maintenanceMode(false),
      loggingEnabled(true),
      resources(100),
      batteryStorage(2),
      nextId(1),
      logPath("factory_log.txt"),
      logFile(logPath, std::ios::app)
{
    if (!logFile) {
        loggingEnabled = false;
    } else {
        log("Factory constructed");
    }
}

Factory::Factory(const Factory& other)
    : Entity(other),
      running(false),
      defenseAlert(other.defenseAlert),
      maintenanceMode(other.maintenanceMode),
      loggingEnabled(other.loggingEnabled),
      resources(other.resources),
      batteryStorage(other.batteryStorage),
      nextId(other.nextId),
      logPath(other.logPath),
      logFile(logPath, std::ios::app)
{
    log("Factory copied");
}

Factory::~Factory() {
    shutdown();
    for (auto* clanker : clankers) {
        delete clanker;
    }
    if (logFile.is_open()) {
        log("Factory destroyed");
        logFile.close();
    }
}

void Factory::produceClanker(Clanker* clankerPtr) {
    if (!clankerPtr) {
        return;
    }

    std::lock_guard<std::mutex> guard(mutex);
    clankers.push_back(clankerPtr);
    clankerPtr->setOwner(this);
    if (auto* worker = dynamic_cast<WorkerClanker*>(clankerPtr)) {
        worker->setFactory(*this);
    } else if (auto* scout = dynamic_cast<ScoutClanker*>(clankerPtr)) {
        scout->setFactory(*this);
    } else if (auto* defender = dynamic_cast<DefenderClanker*>(clankerPtr)) {
        defender->setFactory(*this);
    }
    log("Produced clanker: " + clankerPtr->getName());
}

bool Factory::safeProduceWorker() {
    try {
        unsigned char idToUse = 0;
        {
            std::lock_guard<std::mutex> guard(mutex);
            if (resources < 15) {
                throw std::runtime_error("Insufficient resources");
            }
            resources -= 15;
            idToUse = nextId++;
        }
        auto* worker = new WorkerClanker(idToUse);
        worker->setFactory(*this);
        produceClanker(worker);
        return true;
    } catch (const std::exception& ex) {
        log(std::string("safeProduceWorker failed: ") + ex.what());
        return false;
    }
}

bool Factory::produceBattery(int count) {
    if (count <= 0) {
        return false;
    }
    const int cost = 15 * count;
    if (resources < cost) {
        return false;
    }
    resources -= cost;
    batteryStorage += count;
    log("Produced " + std::to_string(count) + " battery");
    return true;
}

void Factory::updateAll(float dt) {
    std::vector<Clanker*> snapshot;
    {
        std::lock_guard<std::mutex> guard(mutex);
        snapshot = clankers;
    }

    for (auto* clanker : snapshot) {
        if (clanker && !clanker->isDestroyed()) {
            clanker->update(dt);
        }
    }

    std::lock_guard<std::mutex> guard(mutex);
    maintenanceMode = resources < 20;
    defenseAlert = countAlive(clankers) < 2;
}

void Factory::repair(int hp) {
    std::lock_guard<std::mutex> guard(mutex);
    health += hp;
    if (health > MAX_HEALTH) {
        health = MAX_HEALTH;
    }
    active = health > 0;
    log("Factory repaired by " + std::to_string(hp));
}

void Factory::addResources(int delta) {
    std::lock_guard<std::mutex> guard(mutex);
    resources += delta;
    if (resources < 0) {
        resources = 0;
    }
}

int Factory::getResources() const noexcept {
    std::lock_guard<std::mutex> guard(mutex);
    return resources;
}

const std::vector<Clanker*>& Factory::getClankers() const noexcept {
    return clankers;
}

int Factory::getBatteries() const noexcept {
    std::lock_guard<std::mutex> guard(mutex);
    return batteryStorage;
}

void Factory::addBatteries(int diff) {
    std::lock_guard<std::mutex> guard(mutex);
    batteryStorage += diff;
    if (batteryStorage < 0) {
        batteryStorage = 0;
    }
}

void Factory::startProductionThread() {
    if (running) {
        return;
    }
    running = true;
    std::thread([this]() {
        while (running) {
            safeProduceWorker();
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }).detach();
}

void Factory::shutdown() {
    running = false;
}

std::string Factory::defendAgainst(Enemy& enemy) {
    std::lock_guard<std::mutex> guard(mutex);
    if (!enemy.isAlive()) {
        return "Enemy already defeated";
    }

    int defenderCount = 0;
    int workerCount = 0;
    for (auto* clanker : clankers) {
        if (auto* defender = dynamic_cast<DefenderClanker*>(clanker)) {
            ++defenderCount;
        } else if (auto* worker = dynamic_cast<WorkerClanker*>(clanker)) {
            ++workerCount;
        }
    }
    if (const Clanker* first = getFirstActiveClanker()) {
        log(std::string("First responder: ") + first->getName());
    }

    if (defenderCount > 0) {
        enemy.takeDamage(defenderCount * 10);
        log("Defenders retaliated");
        return "Defenders attack the enemy!";
    }
    if (workerCount > 0) {
        enemy.takeDamage(workerCount * 5);
        log("Workers retaliated");
        return "Workers attack the enemy!";
    }

    takeDamage(enemy.getAttack());
    return "Enemy hits the factory!";
}

void Factory::log(const std::string& message) const {
    if (!loggingEnabled || !logFile.is_open()) {
        return;
    }
    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    logFile << '[' << timestamp << "] " << message << '\n';
}

void FactoryInspector::dump(const Factory& factory) {
    std::lock_guard<std::mutex> guard(factory.mutex);
    std::cout << "Inspector -> Resources: " << factory.resources
              << ", Batteries: " << factory.batteryStorage
              << ", Clankers: " << factory.clankers.size() << '\n';
}

}
