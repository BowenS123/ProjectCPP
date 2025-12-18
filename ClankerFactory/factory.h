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

// Vraag 27: own namespace
namespace ClankerSim {

class Enemy;
class Factory;

// Logs factory state in human-readable format (used for debugging/file logging)
std::ostream& operator<<(std::ostream& os, const Factory& f);

class Factory {
public:
    // Vraag 5: maintainability - clean uniform code style with clear naming
    // Game balance constants - tune these to change difficulty/economy
    static constexpr int MAX_HEALTH = 500;
    static constexpr int WORKER_COST = 20;
    static constexpr int SCOUT_COST = 30;
    static constexpr int DEFENDER_COST = 40;

    // Vraag 12: default constructors
    Factory();
    explicit Factory(std::string name);
    Factory(const Factory& other) = delete;

    // Vraag 15: destructor
    ~Factory();
    Factory& operator=(const Factory&) = delete;
    
    // Reset the factory to initial state (used when restarting the game)
    // Vraag 21: default values in function definition
    void reset(const std::string& nameValue = "Unnamed Factory");
    
    // Unit production
    void produceClanker(std::unique_ptr<Clanker> clankerPtr);
    bool produceBattery();  // Returns true if successful
    
    // Game simulation
    // Vraag 20: useful member function
    void updateAll();  // Runs each unit's work() method once per tick
    
    // Factory damage system
    void takeDamage(int dmg);
    void repair(int hp = 10);
    
    // Vraag 5: maintainability - self-documenting function names (addResources, getResources, getBatteries)
    // Resource management
    void addResources(int delta = 0);
    int getResources() const;
    int getBatteries() const;
    void addBatteries(int diff);
    
    // Unit management
    std::vector<const Clanker*> getClankers() const;
    bool giveBatteryTo(unsigned char id);  // Returns true if battery was given
    
    // Generic unit production template - handles all unit types (Worker, Scout, Defender)
    // Vraag 25: useful template function or class (templated unit production)
    template<typename UnitType>
    bool produceUnit(int cost, const std::string& unitName = "Unit") {
        if (resources < cost) { // Check if we can afford it
            return false; // Can't build
        }
        resources -= cost; // Spend the resources
        auto unit = std::make_unique<UnitType>(unitName, 0); // Create new unit (Worker/Scout/Defender)
        unsigned char id = unit->getId(); // Get ID before moving ownership
        produceClanker(std::move(unit)); // Transfer ownership to factory
        log(std::string("Produced ") + unitName + " (ID=" + std::to_string(id) + ")"); // Log creation
        return true; // Success
    }
    
    // Combat system
    std::string defendAgainst(Enemy& enemy);  // Returns detailed combat description
    
    // Getters for UI
    const std::string& getName() const;
    unsigned char getId() const;
    // Vraag 30: const member function (read-only health for UI bar)
    int getHealth() const;
    // Vraag 30: const member function (destruction check)
    bool isDestroyed() const;  // Returns true if health <= 0
    
    // Vraag 26: useful friend function or class (operator<< for logging)
    friend std::ostream& operator<<(std::ostream& os, const Factory& f);

private:
    // Vraag 30: const reference parameter
    void log(const std::string& message) const;

    // Vraag 22: useful member variable (state of simulation)
    std::string name;
    // Vraag 28: memory-efficiënt type (unsigned char)
    unsigned char id;
    int health;
    std::vector<std::unique_ptr<Clanker>> clankers;  // Owns all units (auto-deleted when cleared). Vraag 36: useful container
    
    // Vraag 31: useful boolean flag (logging enabled)
    bool loggingEnabled;
    int resources;        // Used to build units and batteries
    int batteryStorage;   // Batteries that can be assigned to units
    std::string logPath;
    mutable std::ofstream logFile; // Vraag 38: moderne file I/O (ofstream)
};

}  // namespace ClankerSim

#endif // FACTORY_H
