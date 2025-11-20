#include "enemy.h"

namespace ClankerSim {

Enemy::Enemy() : hp(0), attack(0), alive(false), name("Unknown") {}

Enemy::Enemy(int hpValue, int attackValue, std::string nameValue) : hp(hpValue), attack(attackValue), alive(hpValue > 0), name(std::move(nameValue)) {}

Enemy::Enemy(const Enemy& other) : hp(other.hp), attack(other.attack), alive(other.alive), name(other.name) {}

Enemy::~Enemy() = default;

// Called by the factory when defenders retaliate.
void Enemy::takeDamage(int dmg) {
    if (dmg <= 0 || !alive) {
        return;
    }
    hp -= dmg;
    if (hp <= 0) {
        hp = 0;
        alive = false;
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
