#ifndef WORKER_H
#define WORKER_H

#include "clanker.h"

namespace ClankerSim {

class Factory;

// Performs repairs and gathers resources for the factory.
class WorkerClanker final : public Clanker {
public:
    static constexpr int BASE_HP = 100;
    static constexpr int BASE_ENERGY = 100;
    static constexpr int RETALIATION_DAMAGE = 5;

    WorkerClanker();
    
    explicit WorkerClanker(std::string name = "Worker", unsigned char id = 0);
    ~WorkerClanker() override = default;

    void setFactory(Factory& factory);

protected:
    void work() override;

private:
    Factory* factoryRef;
};

}

#endif // WORKER_H
