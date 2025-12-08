#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "../headers/Projectiles.h"
#include "../headers/Utils.h"

class Weapon
{
    const std::string name;
    int damage;
    int bullet_nr;
    float fire_rate;
    float spread_angle;
    float range;
    float bulletSpeed;
    float offset;

    sf::Clock weaponClock;
    sf::Texture &projectileTexture;

public:
    Weapon(const std::string &, const int , const int , const float , const float , const float , const float, const float, sf::Texture &);
    Weapon(const Weapon &);
    Weapon &operator=(const Weapon &);
    ~Weapon() = default;
    friend std::ostream &operator<<(std::ostream &, const Weapon &);

    std::vector<Projectile> fire(const sf::Vector2f &, const sf::Vector2f &);
    void reset();
    void update();
};