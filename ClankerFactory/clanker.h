#ifndef CLANKER_H
#define CLANKER_H

#include <string>

// Vraag 27: Own namespace
namespace ClankerSim {

class Factory;

/*
    BASE CLASS for all units (Workers, Scouts, Defenders)
    Each unit has:
    - Health (HP): Takes damage from enemies
    - Energy: Consumed when working, restored via batteries
    - Name & ID: For identification in UI
    - work(): Virtual method, each subclass implements different behavior
*/

class Clanker {
public:
    // Vraag 12: default constructors
    Clanker();
    // Vraag 13: parameterized constructors
    Clanker(std::string name, unsigned char id, int hp, int energyValue);
    Clanker(const Clanker& other);
    Clanker(Clanker&& other) noexcept; // Vraag 39: exception safety
    // Vraag 15: virtuele destructor voor polymorfie
    virtual ~Clanker() = default;
    
    Clanker& operator=(const Clanker& other);
    Clanker& operator=(Clanker&& other) noexcept;
    
    // Vraag 3: no globals, but statics if needed
    // Auto-ID allocation for units (each unit gets a unique ID)
    // Vraag 28: memory-efficiënt type (unsigned char)
    static unsigned char nextAutoId; // Shared counter across all Clanker instances
    static unsigned char allocateAutoId(); // Returns next available ID

    // Vraag 23: getters
    const std::string& getName() const;
    unsigned char getId() const;
    // Vraag 30: const member function (read-only access to hp)
    int getHp() const;
    // Vraag 30: const member function (read-only status check)
    bool isDestroyed() const;  // Returns true if hp <= 0
    // Vraag 30: const member function (read-only energy check)
    int getEnergy() const;
    
    // Combat
    void takeDamage(int dmg);
    
    // Energy management (gives a battery to restore energy)
    void recharge(Factory& factory);
    
    // Work execution (called once per game tick)
    void doWork();
    
    // Quick check: is energy at maximum?
    // Vraag 24: correct usage of inline function
    inline bool isFullEnergy() const { return energy >= 100; }

protected:
    // Each subclass (Worker, Scout, Defender) implements their own work() behavior
    // Vraag 18: dynamic polymorphism
    virtual void work() = 0;
    
    // Vraag 22: useful member variables (state of unit)
    std::string name;
    // Vraag 28: memory-efficient type (unsigned char)
    unsigned char id;
    int hp; // Current health
    int energy;  // 0-100: consumed by work(), restored by batteries
    
};

}

#endif // CLANKER_H
