#include "worker.h"
#include "factory.h"

namespace ClankerSim {

WorkerClanker::WorkerClanker()
    : WorkerClanker("Worker", 0) {}

WorkerClanker::WorkerClanker(std::string name, unsigned char id)
    : Clanker(std::move(name), id, 100, 80), factoryRef(nullptr) {}

WorkerClanker::WorkerClanker(unsigned char id)
    : WorkerClanker("Worker", id) {}

WorkerClanker::WorkerClanker(const WorkerClanker& other)
    : Clanker(other), factoryRef(other.factoryRef) {}

void WorkerClanker::setFactory(Factory& factory) {
    factoryRef = &factory;
}

void WorkerClanker::work() {
    if (!factoryRef) {
        return;
    }

    if (energy <= 20) {
        recharge(*factoryRef);
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
