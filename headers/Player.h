#pragma once

#include "../headers/Entity.h"
#include "../headers/Weapon.h"
#include "../headers/Utils.h"

#include <nlohmann/json.hpp>
#include <fstream> 
#include <cmath>

class Player : public Entity
{
    explicit Player(sf::Texture &, sf::Texture &);
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;

    Weapon *currentWeapon;
    std::vector<Weapon> weapons;

    static std::unique_ptr<Player> instance;

    bool doTakeDamage(const int &) override;

public:
    static Player &getInstance();
    static Player &Initialize(sf::Texture &, sf::Texture &);
    friend std::ostream &operator<<(std::ostream &, const Player &);
    
    void load();
    void update(const float &, const sf::Vector2f &);
    sf::Vector2i getHealthStatus() const;
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f &);
    std::vector<Projectile> fire(const sf::Vector2f &) const;
};