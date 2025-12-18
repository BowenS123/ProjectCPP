#include "enemy.h"

// Vraag 27: own namespace
namespace ClankerSim {

Enemy::Enemy() : hp(0), attack(0), alive(false), name("Unknown") {}
// Vraag 16: member initialization in constructors (the stuff behind a colon)
Enemy::Enemy(int hpValue, int attackValue, std::string nameValue) : hp(hpValue), attack(attackValue), alive(hpValue > 0), name(std::move(nameValue)) {}
// Vraag 14: copy constructors
Enemy::Enemy(const Enemy& other) : hp(other.hp), attack(other.attack), alive(other.alive), name(other.name) {}
// Vraag 15: destructor
Enemy::~Enemy() = default;

void Enemy::takeDamage(int dmg) {
    if (dmg <= 0 || !alive) { // Ignore if no damage or already dead
        return;
    }
    hp -= dmg; // Apply damage
    if (hp <= 0) { // Check if enemy died
        hp = 0; // Clamp to 0
        alive = false; // Mark as dead
    }
}

int Enemy::getHP() const {
    return hp;
}

int Enemy::getAttack() const {
    return attack;
}

const std::string& Enemy::getName() const {
    return name;
}

bool Enemy::isAlive() const {
    return alive;
}

}
