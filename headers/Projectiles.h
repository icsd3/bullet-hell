#pragma once

#include "../headers/Entities.h"
#include <cmath>

class Projectile : public Entity
{
    sf::Angle rotation;
    int damage;
    sf::Vector2f direction;
    sf::Vector2f origin;
    float range;

public:
    Projectile(const bool &, const sf::Vector2f &, const bool &, const std::string &, const sf::Texture &, float, const int &, const sf::Vector2f &, const float &);
    ~Projectile() = default;

    void load(const sf::Texture &);
    void draw(sf::RenderWindow &);
    bool update(const float &); // Returns true if projectile is out of bounds
    int getDamage() const;

    friend std::ostream &operator<<(std::ostream &os, const Projectile &projectile);
};