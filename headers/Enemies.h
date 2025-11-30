#pragma once

#include "../headers/Entities.h"
#include "../headers/Weapons.h"

class Enemy : public Entity
{
    Enemy(const bool &, const sf::Vector2f &, const bool &, const std::string &, const sf::Texture &, const float, const int &, const Weapon &);
    Weapon weapon;

    sf::RectangleShape maxHealthBar;
    sf::RectangleShape currentHealthBar;

public:
    friend std::ostream &operator<<(std::ostream &, const Enemy &);

    void load() override;
    void draw(sf::RenderWindow &) override;
    bool takeDamage(const int &) override;

    std::vector<Projectile> update(const sf::Vector2f &, const sf::Texture &);
    static Enemy spawnEnemy(const sf::Texture &, const sf::Vector2f &, float, const int &);
};