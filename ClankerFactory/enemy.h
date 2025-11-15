#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
public:
    int hp;
    int attack;
    bool alive;

    Enemy(int hp, int atk) : hp(hp), attack(atk), alive(true) {}

    void takeDamage(int dmg) {
        hp -= dmg;
        if (hp <= 0) {
            alive = false;
        }
    }
};

#endif // ENEMY_H
