#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>

#include "../headers/Projectiles.h"

class Weapon
{
    const std::string name;
    int damage;
    int bullet_nr;
    float fire_rate;
    float spread_angle;
    float range;
    float bulletSpeed;

public:
    Weapon(const std::string& nm, int dmg, int bnr, float fr, float sa, float rg, float bs);
    Weapon(const Weapon&);
    Weapon& operator=(const Weapon&);
    ~Weapon();
    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon);

    Projectile fire(sf::Vector2f&, sf::Vector2f&);
    bool canFire(const float& dt);
};