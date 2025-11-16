#ifndef WORKER_H
#define WORKER_H

#include "clanker.h"

namespace ClankerSim {

class Factory;

class WorkerClanker final : public Clanker {
public:
    WorkerClanker();
    WorkerClanker(std::string name, unsigned char id);
    explicit WorkerClanker(unsigned char id);
    WorkerClanker(const WorkerClanker& other);
    ~WorkerClanker() override = default;

    void setFactory(Factory& factory);

protected:
    void work() override;

private:
    Factory* factoryRef;
};

}

#endif // WORKER_H
