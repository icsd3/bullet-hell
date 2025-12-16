#pragma once

#include "../headers/Projectile.h"
#include "../headers/Weapon.h"

class Gun : public Weapon
{
    int bullet_nr;
    float spread_angle;
    float range;
    float bulletSpeed;
    int projectileTextureID;

    void printDetails(std::ostream &) const override;

public:
    Gun(const std::string &, const int, const float, const float, const int, const float, const float, const float, const int);
    Gun(const Gun &);
    Gun &operator=(const Gun &);

    std::vector<Projectile> fire(const sf::Vector2f &, const sf::Vector2f &) override;
};