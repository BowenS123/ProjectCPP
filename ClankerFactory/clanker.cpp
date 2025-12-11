#include "clanker.h"
#include "factory.h"
#include <utility>

namespace ClankerSim {

unsigned char Clanker::nextAutoId = 1;
unsigned char Clanker::allocateAutoId() {
    return nextAutoId++;
}

Clanker::Clanker() : name("Unnamed"), id(allocateAutoId()), hp(100), energy(50), totalDamageTaken(0) {}
Clanker::Clanker(std::string nameValue, unsigned char identifier, int hpValue, int energyValue) 
    : name(std::move(nameValue)),
      id(identifier ? identifier : allocateAutoId()),
      hp(hpValue), 
      energy(energyValue), 
      totalDamageTaken(0) {}
Clanker::Clanker(const Clanker& other) : name(other.name), id(other.id), hp(other.hp), energy(other.energy), totalDamageTaken(other.totalDamageTaken) {}
Clanker::Clanker(Clanker&& other) noexcept : name(std::move(other.name)), id(other.id), hp(other.hp), energy(other.energy), totalDamageTaken(other.totalDamageTaken) {}
Clanker& Clanker::operator=(const Clanker& other) {
    if (this != &other) {
        name = other.name;
        id = other.id;
        hp = other.hp;
        energy = other.energy;
        totalDamageTaken = other.totalDamageTaken;
    }
    return *this;
}

Clanker& Clanker::operator=(Clanker&& other) noexcept {
    if (this != &other) {
        name = std::move(other.name);
        id = other.id;
        hp = other.hp;
        energy = other.energy;
        totalDamageTaken = other.totalDamageTaken;
    }
    return *this;
}

const std::string& Clanker::getName() const {
    return name;
}

void Clanker::setName(const std::string& newName) {
    name = newName;
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
    totalDamageTaken += dmg;
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

void Clanker::recharge(Factory& factory) {
    if (factory.getBatteries() <= 0 || energy >= 100) {
        return;
    }
    int needed = 100 - energy;
    energy += needed;
    factory.addBatteries(-1);
}

}  // namespace ClankerSim

