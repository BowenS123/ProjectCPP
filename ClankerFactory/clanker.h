#ifndef CLANKER_H
#define CLANKER_H

#include <string>

namespace ClankerSim {

class Factory;

class Clanker {
public:
    Clanker(std::string name, unsigned char id, int hp, int energyValue);
    virtual ~Clanker() = default;

    const std::string& getName() const;
    unsigned char getId() const;
    int getHp() const;
    bool isDestroyed() const;
    void takeDamage(int dmg);

    int getEnergy() const;
    void recharge(Factory& factory);
    void doWork(float dt = 0.0f);

protected:
    virtual void work() = 0;

    std::string name;
    unsigned char id;
    int hp;
    int energy;
};

}

#endif // CLANKER_H
