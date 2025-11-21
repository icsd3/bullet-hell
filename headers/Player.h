#pragma once

#include "../headers/Entities.h"
#include "../headers/Weapons.h"
#include "../ext/json.hpp"
#include <fstream>

#include <cmath>

class Player : public Entity
{
    explicit Player(const sf::Vector2f &, const sf::Texture &tex);
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;

    int maxHealth;
    int currentHealth;

    Weapon currentWeapon;
    std::vector<Weapon> weapons;

public:
    static Player &getInstance(const sf::Vector2f &, const sf::Texture &tex);
    ~Player() = default;
    friend std::ostream &operator<<(std::ostream &os, const Player &player);

    void load(const sf::Texture &);
    void update(const float &, const sf::Vector2f &);
    void draw(sf::RenderWindow &);
    sf::Vector2i getHealthStatus() const;
    sf::Vector2f getPosition() const;
    std::vector<Projectile> fireCurrentWeapon(const sf::Vector2f &, const sf::Texture &tex);
    bool canFireCurrentWeapon();
};