#pragma once

#include "../headers/Entities.h"
#include "../headers/Weapons.h"
#include <nlohmann/json.hpp>
#include <fstream> 

#include <cmath>

class Player : public Entity
{
    explicit Player(const sf::Texture &);
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;

    Weapon *currentWeapon;
    std::vector<Weapon> weapons;

    static std::unique_ptr<Player> instance;
    const sf::Texture &texture;
    sf::RectangleShape collisionBox;

public:
    static Player &getInstance();
    static Player &Initialize(const sf::Texture &);
    ~Player() = default;
    friend std::ostream &operator<<(std::ostream &, const Player &);

    void load();
    void update(const float &, const sf::Vector2f &);
    void draw(sf::RenderWindow &);
    bool takeDamage(const int &);

    sf::Vector2i getHealthStatus() const;
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f &);
    std::vector<Projectile> fire(const sf::Vector2f &, const sf::Texture &);
    bool canFire();
    bool boxCollidesWith(const sf::RectangleShape &) const;
    bool boxCollidesWith(const Object &) const;
};