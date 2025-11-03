#pragma once

#include "../headers/Entities.h"
#include "../headers/Weapons.h"

#include <cmath>

class Player : public Entity
{
    Player();
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    std::vector<Weapon> weapons;

    int maxHealth;
    int currentHealth;

public:
    static Player& getInstance();
    ~Player() = default;
    friend std::ostream& operator<<(std::ostream& os, const Player& player);

    bool hasSprite();
    void loadPlayer();
    void updatePlayer(const float& dt, const sf::Vector2f&);
    void drawPlayer(sf::RenderWindow&);
    sf::Vector2i getHealthStatus() const;
    sf::Vector2f startPosition(const sf::Vector2f&);
};