#ifndef CLANKER_H
#define CLANKER_H

#include "entity.h"

namespace ClankerSim {

class Factory;

class Clanker : public Entity {
public:
    Clanker();
    Clanker(std::string name, unsigned char id, int hp, int energyValue);
    Clanker(const Clanker& other);
    virtual ~Clanker() = default;

    int getEnergy() const noexcept;
    bool hasEnergy() const noexcept {
        return energy > 0;
    }
    void recharge(Factory& factory);
    void setOwner(Factory* factory) {
        owner = factory;
    }

    void update(float dt) override {
        (void)dt; this->work();
    }

protected:
    virtual void work() = 0;

    Factory* owner;
    int energy;
};

inline Clanker::Clanker() : Entity(), owner(nullptr), energy(0) {}

inline Clanker::Clanker(std::string name, unsigned char id, int hp, int energyValue) : Entity(std::move(name), id, hp), owner(nullptr), energy(energyValue) {}

inline Clanker::Clanker(const Clanker& other) : Entity(other), owner(other.owner), energy(other.energy) {}

inline int Clanker::getEnergy() const noexcept {
    return energy;
}

}

#endif // CLANKER_H
