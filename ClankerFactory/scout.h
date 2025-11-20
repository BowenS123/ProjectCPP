#ifndef SCOUT_H
#define SCOUT_H

#include "clanker.h"

namespace ClankerSim {

class Factory;

// Scout unit that scouts and funnels resources back home.
class ScoutClanker final : public Clanker {
public:
    static constexpr int BASE_HP = 80;
    static constexpr int BASE_ENERGY = 100;

    explicit ScoutClanker(std::string name = "Scout", unsigned char id = 0);
    ~ScoutClanker() override = default;

    void setFactory(Factory& factory);

protected:
    void work() override;

private:
    Factory* factoryRef;
};

}

#endif // SCOUT_H
