#pragma once

#include "../headers/Projectile.h"
#include "../headers/Weapon.h"
#include "../headers/SoundManager.h"

class Gun : public Weapon
{
    int bullet_nr;
    float spreadAngle;
    float bulletSpeed;
    int pierce;

    void printDetails(std::ostream &) const override;
    std::vector<std::unique_ptr<Attack>> doAttack(const sf::Vector2f &, const sf::Vector2f &) override;

public:
    Gun(const std::string &, const size_t, const float, const AttackTextureType);
    Gun(const Gun &);
    Gun &operator=(const Gun &);
};