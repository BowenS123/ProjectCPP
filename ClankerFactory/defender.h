#ifndef DEFENDER_H
#define DEFENDER_H

#include "clanker.h"

namespace ClankerSim {

class Factory;

// defender units that focuses on front-line defense.
class DefenderClanker final : public Clanker {
public:
    static constexpr int BASE_HP = 150;
    static constexpr int BASE_ENERGY = 100;
    static constexpr int RETALIATION_DAMAGE = 10;

    DefenderClanker();
    
    explicit DefenderClanker(std::string name = "Defender", unsigned char id = 0);
    ~DefenderClanker() override = default;
    
    void setFactory(Factory& factory);

protected:
    void work() override;

private:
    Factory* factoryRef;
};

}

#endif // DEFENDER_H
