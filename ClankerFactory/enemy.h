#ifndef ENEMY_H
#define ENEMY_H

#include <string>

// Vraag 27: own namespace
namespace ClankerSim {

/*
ENEMY UNIT: Spawned periodically to attack the factory
    Properties: Raw stats only
    - HP: Health before defeat
    - Attack: Damage dealt per attack
    - Alive: Whether this enemy is still a threat

    Enemies get progressively harder:
    - First enemy: 40 HP, 5 ATK
    - Each wave spawn: +5 HP, +0.5 ATK

    Combat: When an enemy attacks, Defenders take damage first,
    then Workers, then  Factory itself.
*/

class Enemy {
public:
    Enemy();
    Enemy(int hp, int attack, std::string name = "Invader");
    Enemy(const Enemy& other);
    
    ~Enemy();

    void takeDamage(int dmg);

    // Getters
    int getHP() const;
    int getAttack() const;
    const std::string& getName() const;
    bool isAlive() const;

private:
    int hp;
    int attack;
    // Vraag 31: useful boolean flag (alive)
    bool alive;
    std::string name; // Vraag 35: string class usage
};

}

#endif // ENEMY_H
