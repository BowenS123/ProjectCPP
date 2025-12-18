// Vraag 6: separate header files - declarations only
#ifndef SCOUT_H
#define SCOUT_H

#include "clanker.h"

// Vraag 27: own namespace
namespace ClankerSim {

class Factory;

/*
    SCOUT UNIT: Fast resource gatherer with salvage ability
    Strategy: Specialized collector, no combat ability

    Behavior (each tick):
    - If energy > 0, consume 5 energy
    - Gather 10 resources per tick

    Special: When enemies die, a Scout goes salvage the wreck in the background
    (handled by MainWindow::kickOffSalvageScan)

    Combat: Cannot attack (0 damage)
*/

class ScoutClanker final : public Clanker {
// Vraag 4: correct protections (public interface for MainWindow to use)
public:
    // Vraag 5: maintainability - meaningful constant names (BASE_HP, BASE_ENERGY)
    static constexpr int BASE_HP = 80;
    static constexpr int BASE_ENERGY = 100;

    // Vraag 12: default constructors
    ScoutClanker();
    // Vraag 13: parameterized constructors
    explicit ScoutClanker(std::string name = "Scout", unsigned char id = 0);
    // Vraag 15: destructor (override)
    ~ScoutClanker() override = default;

    void setFactory(Factory& factory); // Vraag 34: moderne call-by-reference

// Vraag 4: correct protections (protected for subclasses to override)
protected:
    void work() override;

// Vraag 4: correct protections (private - internal state only)
private:
    Factory* factoryRef;
};

}

#endif // SCOUT_H
