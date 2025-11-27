#include "scout.h"
#include "factory.h"

namespace ClankerSim {

ScoutClanker::ScoutClanker(std::string name, unsigned char id) : Clanker(std::move(name), id, BASE_HP, BASE_ENERGY), factoryRef(nullptr) {}

void ScoutClanker::setFactory(Factory& factory) {
    factoryRef = &factory;
}

void ScoutClanker::work() {
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

    factoryRef->addResources(10);
}

}
