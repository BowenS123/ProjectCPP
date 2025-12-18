#include "clanker.h"
#include "factory.h"

#include <utility>

// Vraag 27: own namespace
namespace ClankerSim {

// Vraag 28: unsigned char as ID counter
unsigned char Clanker::nextAutoId = 1; // Shared counter starts at 1
unsigned char Clanker::allocateAutoId() {
    return nextAutoId++; // Returns current ID, then increments for next unit
}

Clanker::Clanker() : name("Unnamed"), id(allocateAutoId()), hp(100), energy(50) {} // Default: unnamed with auto ID
// Vraag 16: member initialization in constructors (the stuff behind a colon)
// Vraag 35: useful string usage (std::string name moved into member)
Clanker::Clanker(std::string nameValue, unsigned char identifier, int hpValue, int energyValue) : name(std::move(nameValue)), id(identifier ? identifier : allocateAutoId()), hp(hpValue), energy(energyValue) {} // Custom values, auto-ID if identifier is 0
// Vraag 14: copy constructors
// Vraag 29: const reference parameter
Clanker::Clanker(const Clanker& other) : name(other.name), id(other.id), hp(other.hp), energy(other.energy) {}
// Vraag 39: exception safety
Clanker::Clanker(Clanker&& other) noexcept : name(std::move(other.name)), id(other.id), hp(other.hp), energy(other.energy) {}
Clanker& Clanker::operator=(const Clanker& other) {
    if (this != &other) {
        name = other.name;
        id = other.id;
        hp = other.hp;
        energy = other.energy;
    }
    return *this;
}

Clanker& Clanker::operator=(Clanker&& other) noexcept {
    if (this != &other) {
        name = std::move(other.name);
        id = other.id;
        hp = other.hp;
        energy = other.energy;
    }
    return *this;
}

// Vraag 23: useful getter (name)
// Vraag 29: const reference return (avoid string copy)
const std::string& Clanker::getName() const {
    return name;
}

// Vraag 23: useful getter (id)
unsigned char Clanker::getId() const {
    return id;
}

// Vraag 23: useful getter (hp)
int Clanker::getHp() const {
    return hp;
}

// Vraag 23: useful getter (status)
bool Clanker::isDestroyed() const {
    return hp <= 0;
}

void Clanker::takeDamage(int dmg) {
    if (dmg <= 0) { // Ignore zero or negative damage
        return;
    }
    hp -= dmg; // Apply damage to current health
    if (hp < 0) { // Prevent negative HP
        hp = 0;
    }
}

// Vraag 23: useful getter (energy)
int Clanker::getEnergy() const {
    return energy;
}

void Clanker::doWork() {
    work(); // Calls the virtual work() method - each subclass has different behavior
}

void Clanker::recharge(Factory& factory) {
    if (factory.getBatteries() <= 0 || energy >= 100) { // Can't recharge if no batteries or already full
        return;
    }
    int needed = 100 - energy; // Calculate how much energy to restore
    energy += needed; // Fill energy to 100
    factory.addBatteries(-1); // Consume one battery from factory storage
}

}  // namespace ClankerSim

