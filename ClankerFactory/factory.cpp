#include "factory.h"
#include "enemy.h"

#include <algorithm>
#include <chrono>
#include <utility>
#include <stdexcept>

namespace ClankerSim {

// Default factory bootstraps with a placeholder name.
Factory::Factory() : Factory("Unnamed Factory") {}

Factory::Factory(std::string nameValue) 
    : name(std::move(nameValue)),
      id(255), 
      health(MAX_HEALTH / 2), 
      clankers(),
      loggingEnabled(true), 
      resources(100), 
      batteryStorage(2), 
      logPath("factory_log.txt"), 
      logFile(logPath, std::ios::app) 
{
    if (!logFile) {
        loggingEnabled = false;
    } else {
        log("Factory constructed");
    }
}

Factory::~Factory() {
    if (logFile.is_open()) {
        log("Factory destroyed");
        logFile.close();
    }
}

const std::string& Factory::getName() const {
    return name;
}
unsigned char Factory::getId() const {
    return id;
}
int Factory::getHealth() const {
    return health;
}
bool Factory::isDestroyed() const {
    return health <= 0;
}

// Central entry point: wires a freshly built clanker into the roster
void Factory::produceClanker(std::unique_ptr<Clanker> clankerPtr) {
    if (!clankerPtr) {
        throw std::invalid_argument("Cannot produce null clanker (exception handling example)");
    }

    Clanker* raw = clankerPtr.get();
    if (auto* worker = dynamic_cast<WorkerClanker*>(raw)) {
        worker->setFactory(*this);
    } else if (auto* scout = dynamic_cast<ScoutClanker*>(raw)) {
        scout->setFactory(*this);
    } else if (auto* defender = dynamic_cast<DefenderClanker*>(raw)) {
        defender->setFactory(*this);
    }
    log("Produced clanker: " + raw->getName());
    clankers.push_back(std::move(clankerPtr));
}

// Converts resources into rechargeable batteries for clankers.
bool Factory::produceBattery() {
    const int cost = 15;
    if (resources < cost) {
        return false;
    }
    resources -= cost;
    batteryStorage += 1;
    log("Produced 1 battery");
    return true;
}

// Runs every clanker once per tick to advance their behavior via virtual work()
void Factory::updateAll(float dt) {
    auto isActive = [](const std::unique_ptr<Clanker>& c) {
        return c && !c->isDestroyed();
    };

    for (auto& uptr : clankers) {
        if (isActive(uptr)) {
            uptr->doWork(dt);
        }
    }
}

// Apply direct damage from enemies when no defenders remain.
void Factory::takeDamage(int dmg) {
    if (dmg <= 0) {
        return;
    }

    health -= dmg;
    
    if (health < 0) {
        health = 0;
    }
    log("Factory damaged for " + std::to_string(dmg));
}

void Factory::repair(int hpValue) {
    health += hpValue;
    if (health > MAX_HEALTH) {
        health = MAX_HEALTH;
    }
    log("Factory repaired by " + std::to_string(hpValue));
}

void Factory::addResources(int delta) {
    resources += delta;
    if (resources < 0) {
        resources = 0;
    }
}

int Factory::getResources() const {
    return resources;
}

std::vector<const Clanker*> Factory::getClankers() const {
    std::vector<const Clanker*> snapshot;
    snapshot.reserve(clankers.size());
    for (const auto& uptr : clankers) {
        if (uptr) {
            snapshot.push_back(uptr.get());
        }
    }
    return snapshot;
}

int Factory::getBatteries() const {
    return batteryStorage;
}

void Factory::addBatteries(int diff) {
    batteryStorage += diff;
    if (batteryStorage < 0) {
        batteryStorage = 0;
    }
}

// Manually assign a battery to a specific clanker by ID.
bool Factory::giveBatteryTo(unsigned char targetId) {
    if (batteryStorage <= 0) {                                              // No batteries available 
        return false;
    }
    for (auto& uptr : clankers) {                                           // Search for clanker by ID
        if (!uptr || uptr->isDestroyed()) {                                 // Skip null or destroyed units
            continue;
        }
        if (uptr->getId() == targetId) {                                    // Found matching clanker
            if (uptr->getEnergy() >= 100) {                                 // Already full energy
                return false;   
            }
            // C++ REQUIREMENT: Call-by-reference parameter
            uptr->recharge(*this);                                          // Recharge clanker 
            log("Battery given to clanker ID " + std::to_string(targetId));
            return true;
        }
    }
    return false;
}

// Using template instead: factory.produceUnit<WorkerClanker>(WORKER_COST, "Worker");
/*
bool Factory::produceWorker() {
    if (resources < WORKER_COST) {                                          // Not enough resources
        return false;
    }
    resources -= WORKER_COST;                                               // Deduct cost
    auto unit = std::make_unique<WorkerClanker>("Worker", 0);               // Create worker
     unsigned char id = unit->getId();                                      // Store ID before transfer
    produceClanker(std::move(unit));                                        // Transfer ownership to factory                               
    log("Produced Worker (ID=" + std::to_string(id) + ")");                 // Log production
    return true;
}
*/

// Using template instead: factory.produceUnit<ScoutClanker>(SCOUT_COST, "Scout");
/*
bool Factory::produceScout() {
    if (resources < SCOUT_COST) {
        return false;
    }
    resources -= SCOUT_COST;
    auto unit = std::make_unique<ScoutClanker>("Scout", 0);
    Clanker* raw = unit.get();
    produceClanker(std::move(unit));
    log("Produced Scout (ID=" + std::to_string(raw->getId()) + ")");
    return true;
}
*/

// Using template instead: factory.produceUnit<DefenderClanker>(DEFENDER_COST, "Defender");
/*
bool Factory::produceDefender() {
    if (resources < DEFENDER_COST) {
        return false;
    }
    resources -= DEFENDER_COST;
    auto unit = std::make_unique<DefenderClanker>("Defender", 0);
    Clanker* raw = unit.get();
    produceClanker(std::move(unit));
    log("Produced Defender (ID=" + std::to_string(raw->getId()) + ")");
    return true;
}
*/

// Resolve an attack step by delegating to powered clankers first.
std::string Factory::defendAgainst(Enemy& enemy) {
    if (!enemy.isAlive()) {
        return "Enemy already defeated";
    }

    int defenderCount = 0;
    int workerCount = 0;
    const Clanker* firstResponder = nullptr;
    std::vector<DefenderClanker*> defendersAlive;
    std::vector<WorkerClanker*> workersAlive;
    
    for (auto& uptr : clankers) {
        Clanker* clanker = uptr.get();
        if (clanker && !clanker->isDestroyed() && !firstResponder) {
            firstResponder = clanker;
        }
        if (auto* defender = dynamic_cast<DefenderClanker*>(clanker)) {
            if (!defender->isDestroyed()) {
                defendersAlive.push_back(defender);
            }
            if (defender->getEnergy() > 0) {
                ++defenderCount;
            }
        } else if (auto* worker = dynamic_cast<WorkerClanker*>(clanker)) {
            if (!worker->isDestroyed()) {
                workersAlive.push_back(worker);
            }
            if (worker->getEnergy() > 0) {
                ++workerCount;
            }
        }
    }

    if (firstResponder) {
        log(std::string("First responder: ") + firstResponder->getName());
    }

    std::vector<std::string> narrative;
    auto addMessage = [&](const std::string& msg) {
        if (!msg.empty()) {
            narrative.push_back(msg);
        }
    };

    std::string outcome;
    if (defenderCount > 0) {
        const int retaliateDamage = defenderCount * DefenderClanker::RETALIATION_DAMAGE;
        enemy.takeDamage(retaliateDamage);
        outcome = "Defenders deal " + std::to_string(retaliateDamage) + " damage.";
        log(outcome);
        addMessage(outcome);
    } else if (workerCount > 0) {
        const int retaliateDamage = workerCount * WorkerClanker::RETALIATION_DAMAGE;
        enemy.takeDamage(retaliateDamage);
        outcome = "Workers deal " + std::to_string(retaliateDamage) + " damage.";
        log(outcome);
        addMessage(outcome);
    }

    if (!enemy.isAlive()) {
        if (outcome.empty()) {
            addMessage("Enemy defeated before striking.");
        }
        std::string joined;
        for (const auto& msg : narrative) {
            if (!joined.empty()) {
                joined += ' ';
            }
            joined += msg;
        }
        return joined;
    }

    int incomingDamage = enemy.getAttack();
    std::string retaliation;
    auto applyDamage = [&](auto& group, const char* label) {
        bool hitSomeone = false;
        for (auto* unit : group) {
            if (!unit || unit->isDestroyed() || incomingDamage <= 0) {
                continue;
            }
            const int before = unit->getHp();
            const int dealt = std::min(before, incomingDamage);
            unit->takeDamage(dealt);
            incomingDamage -= dealt;
            hitSomeone = true;
            const int hpLeft = unit->getHp();
            const std::string detail = unit->getName() + " takes " + std::to_string(dealt) +
                " damage (" + std::to_string(hpLeft) + " HP left)";
            addMessage(detail);
            log(detail);
        }
        if (hitSomeone) {
            if (!retaliation.empty()) {
                retaliation += ' ';
            }
            retaliation += label;
            addMessage(label);
        }
    };

    applyDamage(defendersAlive, "Enemy strikes the defenders!");
    if (incomingDamage > 0) {
        applyDamage(workersAlive, "Enemy strikes the workers!");
    }

    if (incomingDamage > 0) {
        const int remaining = incomingDamage;
        takeDamage(remaining);
        const int factoryHp = health;
        std::string factoryHit = "Enemy hits the factory for " + std::to_string(remaining) +
            " damage (" + std::to_string(factoryHp) + " HP left)";
        addMessage(factoryHit);
        retaliation += (retaliation.empty() ? factoryHit : " " + factoryHit);
        std::string joined;
        for (const auto& msg : narrative) {
            if (!joined.empty()) {
                joined += ' ';
            }
            joined += msg;
        }
        return joined.empty() ? retaliation : joined;
    }

    std::string joined;
    for (const auto& msg : narrative) {
        if (!joined.empty()) {
            joined += ' ';
        }
        joined += msg;
    }
    if (joined.empty()) {
        joined = retaliation;
    }
    return joined.empty() ? outcome : joined;
}

void Factory::log(const std::string& message) const {
    if (!loggingEnabled || !logFile.is_open()) {
        return;
    }

    const auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    logFile << '[' << timestamp << "] " << message << '\n';
}

void Factory::reset(const std::string& nameValue) {
    // Clear all clankers (unique_ptr handles cleanup automatically)
    clankers.clear();

    // Reset identity and state
    name = nameValue;
    id = 255;
    health = MAX_HEALTH / 2;
    resources = 100;
    batteryStorage = 2;

    // Reopen log file (close previous if needed)
    if (logFile.is_open()) {
        log("Factory reset");
        logFile.close();
    }
    loggingEnabled = true;
    logFile.open(logPath, std::ios::app);
    if (!logFile) {
        loggingEnabled = false;
    } else {
        log("Factory reset");
    }
}

// Friend function: operator<< for logging factory state (accesses private members)
std::ostream& operator<<(std::ostream& os, const Factory& f) {
    os << "Factory: " << f.name 
       << " | Health: " << f.health << "/" << Factory::MAX_HEALTH
       << " | Resources: " << f.resources
       << " | Batteries: " << f.batteryStorage
       << " | Units: " << f.clankers.size();
    return os;
}

} // namespace ClankerSim
