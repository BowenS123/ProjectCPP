#include "worker.h"
#include "factory.h"

namespace ClankerSim {

WorkerClanker::WorkerClanker(std::string name, unsigned char id) : Clanker(std::move(name), id, BASE_HP, BASE_ENERGY), factoryRef(nullptr) {}

void WorkerClanker::setFactory(Factory& factory) {
    factoryRef = &factory;
}

void WorkerClanker::work() {
    if (!factoryRef) {
        return;
    }

    if (energy <= 0) {
        return;
    }

    energy -= 5;
    if (energy < 0) {
        energy = 0;
    }

    if (factoryRef->getResources() >= 10 && factoryRef->getHealth() < Factory::MAX_HEALTH) {
        factoryRef->addResources(-10);
        factoryRef->repair(15);
    } else {
        factoryRef->addResources(5);
    }
}

}
