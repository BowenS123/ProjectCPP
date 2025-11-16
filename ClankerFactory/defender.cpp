#include "defender.h"
#include "factory.h"

namespace ClankerSim {

DefenderClanker::DefenderClanker() : DefenderClanker("Defender", 0) {}

DefenderClanker::DefenderClanker(std::string name, unsigned char id) : Clanker(std::move(name), id, 150, 100), factoryRef(nullptr) {}

DefenderClanker::DefenderClanker(unsigned char id) : DefenderClanker("Defender", id) {}

DefenderClanker::DefenderClanker(const DefenderClanker& other) : Clanker(other), factoryRef(other.factoryRef) {}

void DefenderClanker::setFactory(Factory& factory) {
    factoryRef = &factory;
}

void DefenderClanker::work() {
    if (!factoryRef) {
        return;
    }

    energy -= 10;
    if (energy <= 20) {
        recharge(*factoryRef);
    }

    std::cout << getName() << " holds the defensive line\n";
}

}
