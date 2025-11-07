#pragma once

#include "../headers/Entities.h"
#include <cmath>

class Projectile : public Entity
{
    sf::Angle rotation;
    int damage;
    sf::Vector2f target;
    sf::Vector2f direction;

public:
    Projectile(const bool&, const sf::Vector2f&, const bool&, const std::string&, float, const int&, const sf::Vector2f&);
    ~Projectile() = default;

    void loadProjectile(sf::RenderWindow&);
    void drawProjectile(sf::RenderWindow&);
    bool updateProjectile(const float&, sf::RenderWindow&);     // Returns true if projectile is out of bounds
    int getDamage() const;

    friend std::ostream& operator<<(std::ostream& os, const Projectile& projectile);
};