// Vraag 6: separate header files - declarations only
#ifndef DEFENDER_H
#define DEFENDER_H

#include "clanker.h"

// Vraag 27: eigen namespace
namespace ClankerSim {

class Factory;

/*
    DEFENDER UNIT: Tank that absorbs enemy damage first
    Strategy: Expensive but tough, handles combat

    Behavior (each tick):
    - If energy > 0, consume 10 energy
    - Waits on the defensive line (actual combat is handled by Factory::defendAgainst)

    Combat Role: When enemy attacks:
    1. Defenders take damage first (if any are alive with energy)
    2. Each Defender deals 10 retaliation damage to the enemy
    3. If Defenders are gone or out of energy, Workers take damage instead
*/

class DefenderClanker final : public Clanker {
public:
    // Vraag 5: maintainability - meaningful constant names (BASE_HP, BASE_ENERGY, RETALIATION_DAMAGE)
    static constexpr int BASE_HP = 150;
    static constexpr int BASE_ENERGY = 100;
    static constexpr int RETALIATION_DAMAGE = 10;

    // Vraag 12: default constructors
    DefenderClanker();
    // Vraag 13: parameterized constructors
    explicit DefenderClanker(std::string name = "Defender", unsigned char id = 0);
    // Vraag 15: destructor
    ~DefenderClanker() override = default;
    
    void setFactory(Factory& factory); // Vraag 34: moderne call-by-reference

// Vraag 4: correct protections (protected for subclasses to override)
protected:
    void work() override;

// Vraag 4: correct protections (private - internal state only)
private:
    Factory* factoryRef;
};

}

#endif // DEFENDER_H
