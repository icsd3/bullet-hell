#pragma once

#include "../headers/Entities.h"
#include "../headers/Weapons.h"

class Enemy : public Entity
{
    Enemy(const bool &, const sf::Vector2f &, const bool &, const std::string &, const sf::Texture &, const float, const int &, const Weapon &);
    Weapon weapon;

    sf::RectangleShape maxHealthBar;
    sf::RectangleShape currentHealthBar;

    bool canFire();

public:
    static Enemy spawnEnemy(const sf::Texture &, const sf::Vector2f &, float, const int &);

    void load(const sf::Texture &);
    void draw(sf::RenderWindow &);
    std::vector<Projectile> update(const sf::Vector2f &, const sf::Texture &);
    bool takeDamage(const int &);

    Enemy(const Enemy &) = default;
    Enemy &operator=(const Enemy &);
    ~Enemy() = default;
    friend std::ostream &operator<<(std::ostream &, const Enemy &);
};