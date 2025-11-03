#pragma once

#include "../headers/Entities.h"

class Projectile : public Entity
{
    sf::Angle rotation;
    int damage;
    sf::Vector2f target;

public:
    Projectile(const bool&, const sf::Vector2f&, const bool&, const std::string&, float, const int&, const sf::Vector2f&);
    void loadProjectile(sf::RenderWindow&);
    void drawProjectile(sf::RenderWindow&);
    void updateProjectile(const float&);
    friend std::ostream& operator<<(std::ostream& os, const Projectile& projectile);
};