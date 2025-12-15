#pragma once

#include "../headers/Entity.h"
#include "../headers/Weapon.h"

#include <nlohmann/json.hpp>
#include <fstream>

class Player : public Entity
{
    Weapon *currentWeapon;
    std::vector<Weapon> weapons;

public:
    Player(sf::Texture &, sf::Texture &);
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;
    friend std::ostream &operator<<(std::ostream &, const Player &);

    void load();
    void update(const float &, const sf::Vector2f &);
    sf::Vector2i getHealthStatus() const;
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f &);
    std::vector<Projectile> fire(const sf::Vector2f &) const;
};