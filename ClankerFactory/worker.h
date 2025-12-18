// Vraag 6: separate header files - declarations only
#ifndef WORKER_H
#define WORKER_H

#include "clanker.h"

// Vraag 27: own namespace
namespace ClankerSim {

class Factory;

/* 
    WORKER UNIT: Repairs factory OR gathers resources
    Strategy: Cheap to build, balanced stats

    Behavior (each tick):
    - If energy > 0, consume 5 energy
    - If factory is damaged AND has 10+ resources → use resources to repair
    - Otherwise → gather 5 resources for factory

    Combat: Has 5 retaliation damage
*/

class WorkerClanker final : public Clanker {
// Vraag 4: correct protections (public interface for MainWindow to use)
public:
    // Vraag 5: maintainability - meaningful constant names (BASE_HP, BASE_ENERGY, RETALIATION_DAMAGE)
    static constexpr int BASE_HP = 100;
    static constexpr int BASE_ENERGY = 100;
    static constexpr int RETALIATION_DAMAGE = 5;

    // Vraag 12: default constructors
    WorkerClanker();
    // Vraag 13: parameterized constructors
    explicit WorkerClanker(std::string name = "Worker", unsigned char id = 0);
    // Vraag 15: destructor (override)
    ~WorkerClanker() override = default;

    void setFactory(Factory& factory); // Vraag 34: moderne call-by-reference

// Vraag 4: correct protections (protected for subclasses to override)
protected:
    void work() override;

// Vraag 4: correct protections (private - internal state only)
private:
    Factory* factoryRef;
};

}

#endif // WORKER_H
