#pragma once

#include "../headers/Attack.h"

class Projectile : public Attack
{
    void doLoad() override;
    bool doUpdate(const float &) override; // Returns true if projectile range exceeded

public:
    Projectile(const sf::Vector2f &, sf::Texture &, float, const int &, const int &, const sf::Vector2f &, const float &);
    friend std::ostream &operator<<(std::ostream &, const Projectile &);
};