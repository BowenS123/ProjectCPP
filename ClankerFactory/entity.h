#ifndef ENTITY_H
#define ENTITY_H

#include <string>

namespace ClankerSim {

class Entity {
public:
    Entity();
    Entity(std::string name, unsigned char id, int hp);
    Entity(const Entity& other);
    virtual ~Entity() = default;

    virtual void update(float dt) = 0;

    void takeDamage(int dmg = 0);
    bool isDestroyed() const noexcept;
    const std::string& getName() const noexcept;
    unsigned char getId() const noexcept;
    bool isActive() const noexcept;
    int getHealth() const noexcept;
    void setHealth(int hp);

protected:
    std::string name;
    unsigned char id;
    bool active;
    int health;
};

inline Entity::Entity() : name("Unnamed"), id(0), active(false), health(0) {}

inline Entity::Entity(std::string nameValue, unsigned char identifier, int hp) : name(std::move(nameValue)), id(identifier), active(true), health(hp) {}

inline Entity::Entity(const Entity& other) : name(other.name), id(other.id), active(other.active), health(other.health) {}

inline void Entity::takeDamage(int dmg) {
    if (dmg <= 0) {
        return;
    }
    health -= dmg;
    if (health <= 0) {
        health = 0;
        active = false;
    }
}

inline bool Entity::isDestroyed() const noexcept {
    return health <= 0;
}
inline const std::string& Entity::getName() const noexcept {
    return name;
}
inline unsigned char Entity::getId() const noexcept {
    return id;
}
inline bool Entity::isActive() const noexcept {
    return active;
}
inline int Entity::getHealth() const noexcept {
    return health;
}

inline void Entity::setHealth(int hp) {
    health = hp;
    active = health > 0;
}

}

#endif // ENTITY_H
