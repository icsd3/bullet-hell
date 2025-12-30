#pragma once

#include "../headers/Projectile.h"
#include "../headers/Weapon.h"

enum class ProjectileTextureType
{
    Player,
    Enemy,
    Boss
};

class Gun : public Weapon
{
    int bullet_nr;
    float spread_angle;
    float range;
    float bulletSpeed;
    ProjectileTextureType projectileTextureID;

    void printDetails(std::ostream &) const override;
    std::vector<std::unique_ptr<Attack>> doFire(const sf::Vector2f &, const sf::Vector2f &) override;

public:
    Gun(const std::string &, const int, const float, const float, const int, const float, const float, const float, const ProjectileTextureType, const sf::Texture &);
    Gun(const Gun &);
    Gun &operator=(const Gun &);
};