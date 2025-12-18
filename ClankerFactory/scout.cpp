// Vraag 6: separate header files - implementations in .cpp file
#include "scout.h"
#include "factory.h"

// Vraag 27: own namespace
namespace ClankerSim {

// Vraag 17 constructor forwarding
ScoutClanker::ScoutClanker() : ScoutClanker("Scout", 0) {}

// Vraag 16: member initialization in constructors (the stuff behind a colon)
ScoutClanker::ScoutClanker(std::string name, unsigned char id) : Clanker(std::move(name), id, BASE_HP, BASE_ENERGY), factoryRef(nullptr) {} // Vraag 37: nullptr (not linked to a factory yet)

// Vraag 34: moderne call-by-reference
void ScoutClanker::setFactory(Factory& factory) {
    factoryRef = &factory;
}

// Vraag 18: dynamic polymorphism
void ScoutClanker::work() {
    if (!factoryRef) { // Safety check - make sure we're linked to a factory
        return;
    }

    if (energy <= 0) { // No energy = can't work
        return;
    }

    energy -= 5; // Consume energy to work
    if (energy < 0) { // Clamp to 0
        energy = 0;
    }

    factoryRef->addResources(10);
}

}
