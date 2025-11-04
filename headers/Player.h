#pragma once

#include "../headers/Entities.h"
#include "../headers/Weapons.h"

#include <cmath>

class Player : public Entity
{
    explicit Player(const sf::Vector2f&);
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    int maxHealth;
    int currentHealth;

    Weapon currentWeapon;
    std::vector<Weapon> weapons;

public:
    static Player& getInstance(const sf::Vector2f&);
    ~Player() = default;
    friend std::ostream& operator<<(std::ostream& os, const Player& player);

    void loadPlayer(sf::RenderWindow&);
    void updatePlayer(const float& dt, const sf::Vector2f&);
    void drawPlayer(sf::RenderWindow&);
    sf::Vector2i getHealthStatus() const;
    Projectile fireCurrentWeapon(const sf::Vector2f&);
    bool canFireCurrentWeapon(const float& dt);
};