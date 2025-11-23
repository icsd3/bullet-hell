#pragma once

#include "../headers/Entities.h"
#include "../headers/Weapons.h"
#include <nlohmann/json.hpp>
#include <fstream> 

#include <cmath>

class Player : public Entity
{
    explicit Player(const sf::Vector2f &, const sf::Texture &);
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;

    int maxHealth;
    int currentHealth;

    Weapon currentWeapon;
    std::vector<Weapon> weapons;

public:
    static Player &getInstance(const sf::Vector2f &, const sf::Texture &);
    ~Player() = default;
    friend std::ostream &operator<<(std::ostream &, const Player &);

    void load(const sf::Texture &);
    void update(const float &, const sf::Vector2f &);
    void draw(sf::RenderWindow &);
    bool takeDamage(const int &);

    sf::Vector2i getHealthStatus() const;
    sf::Vector2f getPosition() const;
    std::vector<Projectile> fire(const sf::Vector2f &, const sf::Texture &);
    bool canFire();
};