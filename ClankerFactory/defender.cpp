#include "defender.h"
#include "factory.h"

namespace ClankerSim {

DefenderClanker::DefenderClanker(std::string name, unsigned char id) : Clanker(std::move(name), id, BASE_HP, BASE_ENERGY), factoryRef(nullptr) {}

void DefenderClanker::setFactory(Factory& factory) {
    factoryRef = &factory;
}

void DefenderClanker::work() {
    if (!factoryRef) {
        return;
    }

    if (energy <= 15) {
        recharge(*factoryRef);
    }

    if (energy <= 0) {
        return;
    }

    energy -= 10;
    if (energy < 0) {
        energy = 0;
    }

    std::cout << getName() << " holds the defensive line\n";
}

}
