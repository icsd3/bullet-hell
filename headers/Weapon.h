#pragma once

#include "../headers/Projectile.h"

class Weapon
{
    std::string name;

protected:
    int damage;
    float attackSpeed;
    sf::Clock weaponClock;

public:
    Weapon(const std::string &, const int, const float, const float);
    Weapon(const Weapon &);
    Weapon &operator=(const Weapon &);
    virtual ~Weapon() = default;
    friend std::ostream &operator<<(std::ostream &, const Weapon &);

    virtual std::vector<Projectile> fire(const sf::Vector2f &, const sf::Vector2f &) = 0;

    void reset();
    void update();
};