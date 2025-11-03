#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>

class Weapon
{
    const std::string name;
    int damage;
    int bullet_nr;
    float fire_rate;
    float spread_angle;
    float range;

    // void fire(sf::Vector2f&);
public:
    Weapon(const std::string& nm, int dmg, int bnr, float fr, float sa, float rg);
    Weapon(const Weapon&);
    Weapon& operator=(const Weapon&);
    ~Weapon();
    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon);
};