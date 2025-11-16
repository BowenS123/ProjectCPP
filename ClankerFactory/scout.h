#ifndef SCOUT_H
#define SCOUT_H

#include "clanker.h"

namespace ClankerSim {

class Factory;

class ScoutClanker final : public Clanker {
public:
    ScoutClanker();
    ScoutClanker(std::string name, unsigned char id);
    explicit ScoutClanker(unsigned char id);
    ScoutClanker(const ScoutClanker& other);
    ~ScoutClanker() override = default;

    void setFactory(Factory& factory);

protected:
    void work() override;

private:
    Factory* factoryRef;
};

}

#endif // SCOUT_H
