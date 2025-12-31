#pragma once

#include "../headers/Stab.h"
#include "../headers/Weapon.h"

class Spear : public Weapon
{
    float duration;
    float stabSpeed;

    void printDetails(std::ostream &) const override;
    std::vector<std::unique_ptr<Attack>> doAttack(const sf::Vector2f &, const sf::Vector2f &) override;

public:
    Spear(const std::string &, const size_t, AttackTextureType);
    Spear(const Spear &);
    Spear &operator=(const Spear &);
};
