#include "factory.h"
#include "enemy.h"

#include <algorithm>
#include <chrono>
#include <utility>

namespace ClankerSim {

// Default factory bootstraps with a placeholder name.
Factory::Factory() : Factory("Unnamed Factory") {}

Factory::Factory(std::string nameValue) : name(std::move(nameValue)), id(255), health(MAX_HEALTH / 2), clankers(), loggingEnabled(true), resources(100), batteryStorage(2), logPath("factory_log.txt"), logFile(logPath, std::ios::app)
{
    if (!logFile) {
        loggingEnabled = false;
    } else {
        log("Factory constructed");
    }
}

Factory::~Factory() {
    for (auto* clanker : clankers) {
        delete clanker;
    }
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

// Central entry point that wires a freshly built clanker into the roster.
void Factory::produceClanker(Clanker* clankerPtr) {
    if (!clankerPtr) {
        return;
    }

    clankers.push_back(clankerPtr);
    if (auto* worker = dynamic_cast<WorkerClanker*>(clankerPtr)) {
        worker->setFactory(*this);
    } else if (auto* scout = dynamic_cast<ScoutClanker*>(clankerPtr)) {
        scout->setFactory(*this);
    } else if (auto* defender = dynamic_cast<DefenderClanker*>(clankerPtr)) {
        defender->setFactory(*this);
    }
    log("Produced clanker: " + clankerPtr->getName());
}

// Converts resources into rechargeable batteries for clankers.
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

// Runs every clanker once per tick to advance their behavior.
void Factory::updateAll(float dt) {
    std::vector<Clanker*> snapshot = clankers;

    for (auto* clanker : snapshot) {
        if (clanker && !clanker->isDestroyed()) {
            clanker->doWork(dt);
        }
    }
}

void Factory::update(float dt) {
    updateAll(dt);
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

const std::vector<Clanker*>& Factory::getClankers() const {
    return clankers;
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
    if (batteryStorage <= 0) {
        return false;
    }
    for (auto* clanker : clankers) {
        if (!clanker || clanker->isDestroyed()) {
            continue;
        }
        if (clanker->getId() == targetId) {
            const int current = clanker->getEnergy();
            if (current >= 100) {
                return false;
            }
            const int needed = 100 - current;
            clanker->takeDamage(0);
            clanker->recharge(*this);
            if (batteryStorage > 0) {
            }
            return true;
        }
    }
    return false;
}

bool Factory::produceWorker() {
    if (resources < WORKER_COST) {
        return false;
    }
    resources -= WORKER_COST;
    auto* unit = new WorkerClanker("Worker", 0);
    unit->setFactory(*this);
    produceClanker(unit);
    log("Produced Worker (ID=" + std::to_string(unit->getId()) + ")");
    return true;
}

bool Factory::produceScout() {
    if (resources < SCOUT_COST) {
        return false;
    }
    resources -= SCOUT_COST;
    auto* unit = new ScoutClanker("Scout", 0);
    unit->setFactory(*this);
    produceClanker(unit);
    log("Produced Scout (ID=" + std::to_string(unit->getId()) + ")");
    return true;
}

bool Factory::produceDefender() {
    if (resources < DEFENDER_COST) {
        return false;
    }
    resources -= DEFENDER_COST;
    auto* unit = new DefenderClanker("Defender", 0);
    unit->setFactory(*this);
    produceClanker(unit);
    log("Produced Defender (ID=" + std::to_string(unit->getId()) + ")");
    return true;
}

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
    for (auto* clanker : clankers) {
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

} // namespace ClankerSim
