#pragma once

#include "../headers/Attack.h"

class Slash : public Attack
{
    float duration;
    sf::Angle arcAngle;

    void doLoad() override;
    bool doUpdate(const float &) override;

public:
    Slash(const sf::Vector2f &, sf::Texture &, const int &, const sf::Vector2f &, const float &, const float &, const float &);
    friend std::ostream &operator<<(std::ostream &os, const Slash &slash);
};