#ifndef ENEMY_H
#define ENEMY_H

#include <string>

namespace ClankerSim {

class Enemy {
public:
    Enemy();
    Enemy(int hp, int attack, std::string name = "Invader");
    Enemy(const Enemy& other);
    ~Enemy();

    void takeDamage(int dmg);

    int getHP() const noexcept;
    int getAttack() const noexcept;
    const std::string& getName() const noexcept;
    bool isAlive() const noexcept;

private:
    int hp;
    int attack;
    bool alive;
    std::string name;
};

}

#endif // ENEMY_H
