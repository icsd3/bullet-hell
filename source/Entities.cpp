#include "../headers/Entities.h"
#include <iostream>

Entity::Entity(const bool &ec, const sf::Vector2f &pos, const bool &ori, const std::string &tf, const sf::Texture &tex, const float spd, const int mh)
    : Object(ec, pos, ori, tf, tex), speed(spd), maxHealth(mh), currentHealth(mh)
{
    isEntity = true;
}

Entity::Entity(const Entity &other)
    : Object(other), speed(other.speed), maxHealth(other.maxHealth), currentHealth(other.currentHealth)
{
    isEntity = true;
}

Entity &Entity::operator=(const Entity &other)
{
    if (this != &other)
    {
        Object::operator=(other);
        speed = other.speed;
        maxHealth = other.maxHealth;
        currentHealth = other.currentHealth;
        isEntity = true;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Entity &entity)
{
    os << "Entity (";
    os << static_cast<const Object &>(entity);
    os << ", Speed: " << entity.speed << ")";
    return os;
}