#include "factory.h"
#include "enemy.h"

#include <algorithm>
#include <chrono>
#include <stdexcept>

// Vraag 27: own namespace
namespace ClankerSim { 

// Default factory bootstraps with a placeholder name.
// Vraag 17: constructor forwarding
Factory::Factory() : Factory("Unnamed Factory") {}

// Vraag 16: member initialization in constructors (the stuff behind a colon)
Factory::Factory(std::string nameValue) : name(std::move(nameValue)), id(255), health(MAX_HEALTH / 2), clankers(), loggingEnabled(true), resources(100), batteryStorage(2), logPath("factory_log.txt"), logFile(logPath, std::ios::app)
{
    if (!logFile) { // Check if file opened successfully
        loggingEnabled = false; // Disable logging if file can't be opened
    } else {
        log("Factory constructed"); // Write first log entry
    }
}

// Vraag 15: destructor (sluit file netjes)
Factory::~Factory() {
    if (logFile.is_open()) { // Check if log file is still open
        log("Factory destroyed"); // Write final log entry
        logFile.close(); // Close file properly before program ends
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
    if (!clankerPtr) { // Safety check - ensure pointer is not null
        // Vraag 39: useful exception handling (throw on incorrect input)
        throw std::invalid_argument("Cannot produce null clanker (exception handling example)");
    }

    Clanker* raw = clankerPtr.get(); // Get raw pointer (unique_ptr still owns it)
    if (auto* worker = dynamic_cast<WorkerClanker*>(raw)) { // Check if it's a Worker
        worker->setFactory(*this); // Link worker to this factory
    } else if (auto* scout = dynamic_cast<ScoutClanker*>(raw)) { // Check if it's a Scout
        scout->setFactory(*this); // Link scout to this factory
    } else if (auto* defender = dynamic_cast<DefenderClanker*>(raw)) { // Check if it's a Defender
        defender->setFactory(*this); // Link defender to this factory
    }
    log("Produced clanker: " + raw->getName()); // Log creation event
    clankers.push_back(std::move(clankerPtr)); // Transfer ownership to our vector
}

// Converts resources into rechargeable batteries for clankers.
// Vraag 20: useful member function (resources -> battery)
bool Factory::produceBattery() {
    const int cost = 15; // Each battery costs 15 resources
    if (resources < cost) { // Not enough resources to build
        return false; // Tell caller we failed
    }
    resources -= cost; // Spend the resources
    batteryStorage += 1; // Add battery to storage
    log("Produced 1 battery"); // Log the production
    return true; // Tell caller we succeeded
}

// Runs every clanker once per tick to advance their behavior via virtual work()
// Vraag 40: nuttige lambda (filter actief)
void Factory::updateAll() {
    // Vraag 29: const reference parameter (avoid copying unique_ptr)
    auto isActive = [](const std::unique_ptr<Clanker>& c) { // Lambda checks if unit is alive
        return c && !c->isDestroyed(); // Valid pointer AND not destroyed
    };

    for (auto& uptr : clankers) { // Loop through all units
        if (isActive(uptr)) { // Only process living units
            uptr->doWork(); // Tell unit to do its job (Worker/Scout/Defender specific)
        }
    }
}

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
    std::vector<const Clanker*> snapshot; // Vraag 36: useful container (read-only snapshot for UI)
    snapshot.reserve(clankers.size());
    // Vraag 29: const reference in range-loop (avoid unnecessary copies)
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
            if (uptr->isFullEnergy()) {
                return false;   
            }
            uptr->recharge(*this);                                          // Recharge clanker 
            log("Battery given to clanker ID " + std::to_string(targetId));
            return true;
        }
    }
    return false;
}

/*
REMOVED - REPLACED BY TEMPLATE
These three functions were moved to a single generic template function
because they performed nearly identical operations. The template approach is better because:
- One function instead of three
- Extensible: Easy to add new unit

bool Factory::produceWorker() {
    if (resources < WORKER_COST) {
        return false;
    }
    resources -= WORKER_COST;
    auto unit = std::make_unique<WorkerClanker>("Worker", 0);
    unsigned char id = unit->getId();
    produceClanker(std::move(unit));
    log("Produced Worker (ID=" + std::to_string(id) + ")");
    return true;
}

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
// Vraag 34: moderne call-by-reference
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

    std::vector<std::string> narrative; // Collect messages describing what happened in combat
    // Vraag 29: const reference lambda parameter (efficient message handling)
    // Vraag 40: lambda function (captures narrative by reference)
    auto addMessage = [&](const std::string& msg) { // Helper lambda to add combat messages
        if (!msg.empty()) { // Only add non-empty messages
            narrative.push_back(msg); // Add to list
        }
    };

    std::string outcome;
    if (defenderCount > 0) { // Defenders with energy fight back first
        const int retaliateDamage = defenderCount * DefenderClanker::RETALIATION_DAMAGE; // Each defender deals 10 damage
        enemy.takeDamage(retaliateDamage); // Apply damage to enemy
        outcome = "Defenders deal " + std::to_string(retaliateDamage) + " damage.";
        log(outcome);
        addMessage(outcome);
    } else if (workerCount > 0) { // If no defenders, workers fight back
        const int retaliateDamage = workerCount * WorkerClanker::RETALIATION_DAMAGE; // Each worker deals 5 damage
        enemy.takeDamage(retaliateDamage); // Apply damage to enemy
        outcome = "Workers deal " + std::to_string(retaliateDamage) + " damage.";
        log(outcome);
        addMessage(outcome);
    }

    if (!enemy.isAlive()) { // Check if we killed the enemy
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
    // Vraag 19: useful usage of "this" (needed for member access)
    // Vraag 40: lambda function (generic damage applicator with captures)
    auto applyDamage = [&, this](auto& group, const char* label) {
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
            const std::string detail = unit->getName() + " takes " + std::to_string(dealt) + " damage (" + std::to_string(hpLeft) + " HP left)";
            addMessage(detail);
            this->log(detail);
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
// Vraag 29: const reference parameter (avoid string copy on every log call)
void Factory::log(const std::string& message) const {
    if (!loggingEnabled || !logFile.is_open()) {
        return;
    }

    const auto timestamp = std::chrono::system_clock::now().time_since_epoch().count(); // Vraag 49: external library (std::chrono)
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
