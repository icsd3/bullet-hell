#pragma once

#include "../headers/Entity.h"
#include "../headers/Utils.h"

#include <cmath>

class Projectile : public Entity
{
    sf::Angle rotation;
    int damage;
    sf::Vector2f direction;
    sf::Vector2f origin;
    float range;

    int doHits(const Entity &other) const override;

public:
    Projectile(const sf::Vector2f &, sf::Texture &, float, const int &, const sf::Vector2f &, const float &);    
    friend std::ostream &operator<<(std::ostream &, const Projectile &);

    void load();
    bool update(const float &); // Returns true if projectile range exceeded
};