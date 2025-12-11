#ifndef ENEMY_H
#define ENEMY_H

#include <string>

namespace ClankerSim {

// opponent used by the UI wave spawner.
class Enemy {
public:
    Enemy();
    
    Enemy(int hp, int attack, std::string name = "Invader");
    
    Enemy(const Enemy& other);
    
    ~Enemy();

    void takeDamage(int dmg);

    int getHP() const;
    int getAttack() const;
    const std::string& getName() const;
    bool isAlive() const; 

private:
    int hp;
    int attack;
    bool alive;
    std::string name;
};

}

#endif // ENEMY_H
