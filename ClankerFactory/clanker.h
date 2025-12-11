#ifndef CLANKER_H
#define CLANKER_H

#include <string>
#include <memory>

namespace ClankerSim {

class Factory;

class Clanker {
public:
    Clanker();
    Clanker(std::string name, unsigned char id, int hp, int energyValue);
    Clanker(const Clanker& other);
    Clanker(Clanker&& other) noexcept;
    virtual ~Clanker() = default;
    
    Clanker& operator=(const Clanker& other);
    Clanker& operator=(Clanker&& other) noexcept;
    
    static unsigned char nextAutoId;
    static unsigned char allocateAutoId();

    const std::string& getName() const;
    unsigned char getId() const;
    int getHp() const;
    bool isDestroyed() const;
    int getEnergy() const;
    
    void setName(const std::string& newName);
    void takeDamage(int dmg);
    void recharge(Factory& factory);
    void doWork(float dt = 1.0f);

    inline bool isLowEnergy() const { return energy < 30; }
    inline bool isFullEnergy() const { return energy >= 100; }

protected:
    virtual void work() = 0;
    std::string name;
    unsigned char id; 
    int hp;
    int energy;
    
    int totalDamageTaken = 0;
};

}

#endif // CLANKER_H
