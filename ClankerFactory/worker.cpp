// Vraag 6: separate header files - implementations in .cpp file
#include "worker.h"
#include "factory.h"

// Vraag 27: own namespace
namespace ClankerSim {

// Vraag 17: constructor forwarding
WorkerClanker::WorkerClanker() : WorkerClanker("Worker", 0) {}

// Vraag 16: member initialization in constructors (the stuff behind a colon)
WorkerClanker::WorkerClanker(std::string name, unsigned char id) : Clanker(std::move(name), id, BASE_HP, BASE_ENERGY), factoryRef(nullptr) {} // Vraag 37: nullptr (no factory attached yet)

// Vraag 34: moderne call-by-reference
void WorkerClanker::setFactory(Factory& factory) {
    factoryRef = &factory; // Store pointer so we can call factory methods later
}

// Vraag 18: dynamic polymorphism
void WorkerClanker::work() {
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

    if (factoryRef->getResources() >= 10 && factoryRef->getHealth() < Factory::MAX_HEALTH) {
        factoryRef->addResources(-10); // Spend 10 resources
        factoryRef->repair(15); // Repair factory for 15 HP
    } else {
        factoryRef->addResources(5); // Gather 5 resources instead
    }
}

}
