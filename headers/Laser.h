#pragma once

#include "../headers/Weapon.h"
#include "../headers/Beam.h"

class Laser : public Weapon
{
    float chargeUpTime;
    float duration;
    float width;

    void printDetails(std::ostream &) const override;
    std::vector<std::unique_ptr<Attack>> doAttack(const sf::Vector2f &, const sf::Vector2f &) override;

public:
    Laser(const std::string &, const size_t, const float, const AttackTextureType);
    Laser(const Laser &);
    Laser &operator=(const Laser &);
};