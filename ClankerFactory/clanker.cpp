#include "clanker.h"
#include "factory.h"

namespace ClankerSim {

void Clanker::recharge(Factory& factory) {
    if (factory.getBatteries() <= 0 || energy >= 100) {
        return;
    }
    int needed = 100 - energy;
    energy += needed;
    factory.addBatteries(-1);
}

}
