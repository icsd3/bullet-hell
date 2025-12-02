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
    Projectile(const sf::Vector2f &, const bool &, sf::Texture &, float, const int &, const sf::Vector2f &, const float &);    
    friend std::ostream &operator<<(std::ostream &, const Projectile &);

    void load() override;
    int hits(const Entity &other) const override;
    bool update(const float &); // Returns true if projectile range exceeded
};