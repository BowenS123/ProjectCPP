#include "scout.h"
#include "factory.h"

namespace ClankerSim {

ScoutClanker::ScoutClanker()
    : ScoutClanker("Scout", 0) {}

ScoutClanker::ScoutClanker(std::string name, unsigned char id)
    : Clanker(std::move(name), id, 80, 60), factoryRef(nullptr) {}

ScoutClanker::ScoutClanker(unsigned char id)
    : ScoutClanker("Scout", id) {}

ScoutClanker::ScoutClanker(const ScoutClanker& other)
    : Clanker(other), factoryRef(other.factoryRef) {}

void ScoutClanker::setFactory(Factory& factory) {
    factoryRef = &factory;
}

void ScoutClanker::work() {
    if (!factoryRef) {
        return;
    }

    if (energy <= 10) {
        recharge(*factoryRef);
        return;
    }

    energy -= 5;
    factoryRef->addResources(10);
}

}
