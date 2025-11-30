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
    
    friend std::ostream &operator<<(std::ostream &, const Projectile &);

    void load() override;
    int hits(const Entity &other) const override;
    bool update(const float &); // Returns true if projectile is out of bounds
};