#pragma once

#include "../headers/Entities.h"
#include "../headers/Weapons.h"

#include <cmath>

class Player : public Entity
{
    Player();
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    friend class Game;

    std::vector<Weapon> weapons;

    int maxHealth;
    int currentHealth;

    void loadPlayer();
    void updatePlayer(sf::Clock&, const sf::Vector2f&);

public:
    static Player& getInstance();
    ~Player() = default;
    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};