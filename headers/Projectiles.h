#pragma once

#include "../headers/Entities.h"
#include "../headers/Utils.h"

#include <cmath>

class Projectile : public Entity
{
    sf::Angle rotation;
    int damage;
    sf::Vector2f direction;
    sf::Vector2f origin;
    float range;

    void doLoad() override;
    int doHits(const Entity &other) const override;

public:
    Projectile(const sf::Vector2f &, const bool &, sf::Texture &, float, const int &, const sf::Vector2f &, const float &);    
    friend std::ostream &operator<<(std::ostream &, const Projectile &);

    bool update(const float &); // Returns true if projectile range exceeded
};