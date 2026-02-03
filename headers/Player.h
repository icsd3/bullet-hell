#pragma once

#include "../headers/Gun.h"
#include "../headers/Sword.h"
#include "../headers/Spear.h"
#include "../headers/Laser.h"


#include <nlohmann/json.hpp>
#include <fstream>

class Player : public Entity
{
    size_t currentWeapon;
    std::vector<std::unique_ptr<Weapon>> weapons;

    void doDraw(sf::RenderWindow &) const override;

public:
    Player();
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;
    friend std::ostream &operator<<(std::ostream &, const Player &);

    void load();
    void update(const float &, const sf::Vector2f &, const sf::Vector2f &);
    sf::Vector2i getHealthStatus() const;
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f &, const sf::Vector2f &);
    void cycleWeapon(bool);
    std::vector<std::unique_ptr<Attack>> attack(const sf::Vector2f &) const;
    void heal(int amount);
    void addWeapon(std::unique_ptr<Weapon> weapon);
};