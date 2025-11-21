#include "clanker.h"
#include "factory.h"

namespace ClankerSim {

Clanker::Clanker(std::string nameValue, unsigned char identifier, int hpValue, int energyValue) : name(std::move(nameValue)), id(identifier), hp(hpValue), energy(energyValue) {}

const std::string& Clanker::getName() const {
    return name;
}

unsigned char Clanker::getId() const {
    return id;
}

int Clanker::getHp() const {
    return hp;
}

bool Clanker::isDestroyed() const {
    return hp <= 0;
}

void Clanker::takeDamage(int dmg) {
    if (dmg <= 0) {
        return;
    }
    hp -= dmg;
    if (hp < 0) {
        hp = 0;
    }
}

int Clanker::getEnergy() const {
    return energy;
}

void Clanker::doWork(float dt) {
    (void)dt;
    work();
}

// battery system for clankers
void Clanker::recharge(Factory& factory) {
    if (factory.getBatteries() <= 0 || energy >= 100) {
        return;
    }
    int needed = 100 - energy;
    energy += needed;
    factory.addBatteries(-1);
}

}
