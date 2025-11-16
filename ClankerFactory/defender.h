#ifndef DEFENDER_H
#define DEFENDER_H

#include "clanker.h"

namespace ClankerSim {

class Factory;

class DefenderClanker final : public Clanker {
public:
    DefenderClanker();
    DefenderClanker(std::string name, unsigned char id);
    explicit DefenderClanker(unsigned char id);
    DefenderClanker(const DefenderClanker& other);
    ~DefenderClanker() override = default;

    void setFactory(Factory& factory);

protected:
    void work() override;

private:
    Factory* factoryRef;
};

}

#endif // DEFENDER_H
