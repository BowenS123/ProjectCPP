// Vraag 6: separate header files - implementations in .cpp file
#include "defender.h"
#include "factory.h"

// Vraag 27: Own namespace
namespace ClankerSim {

// Vraag 17: constructor forwarding
DefenderClanker::DefenderClanker() : DefenderClanker("Defender", 0) {}

// Vraag 16: member initialization in constructors (the stuff behind a colon)
DefenderClanker::DefenderClanker(std::string name, unsigned char id) : Clanker(std::move(name), id, BASE_HP, BASE_ENERGY), factoryRef(nullptr) {} // Vraag 37: nullptr (clear default before setFactory)

// Vraag 34: moderne call-by-reference
void DefenderClanker::setFactory(Factory& factory) {
    factoryRef = &factory;
}

// Vraag 18: dynamic polymorphism
void DefenderClanker::work() {
    if (!factoryRef) { // Safety check - make sure we're linked to a factory
        return;
    }

    if (energy <= 0) { // No energy = can't defend
        return;
    }

    energy -= 10; // Defenders consume more energy (10 vs 5) because they're heavy
    if (energy < 0) { // Clamp to 0
        energy = 0;
    }
}

}
