#pragma once

#include "../headers/Slash.h"
#include "../headers/Weapon.h"

class Sword : public Weapon
{
    float duration;
    float arcAngle;

    void printDetails(std::ostream &) const override;
    std::vector<std::unique_ptr<Attack>> doAttack(const sf::Vector2f &, const sf::Vector2f &) override;

public:
    Sword(const std::string &, const size_t);
    Sword(const Sword &);
    Sword &operator=(const Sword &);
};