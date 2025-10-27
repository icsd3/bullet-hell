#pragma once

#include "../headers/Entities.h"
#include "../headers/AssetLoader.h"

#include <cmath>

class Weapon
{
    const std::string name;
    int damage;
    int bullet_nr;
    float fire_rate;
    float spread_angle;
    float range;
public:
    Weapon(const std::string& nm, int dmg, int bnr, float fr, float sa, float rg);
    Weapon(const Weapon&);
    Weapon& operator=(const Weapon&);
    ~Weapon();
    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon);
};

class Player : public Entity
{
    Player();
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    friend class Game;

    std::vector<Weapon> weapons;

    int max_health;
    int current_health;

    void loadPlayer();
    void updatePlayer(sf::Clock&, sf::Vector2f&);
public:
    static Player& getInstance();
    ~Player() = default;
    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};