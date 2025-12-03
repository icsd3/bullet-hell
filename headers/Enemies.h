#pragma once

#include "../headers/Entities.h"
#include "../headers/Weapons.h"

class Enemy : public Entity
{
    Enemy(const sf::Vector2f &, const bool &, sf::Texture &, const float, const int &, const Weapon &);
    Weapon weapon;

    sf::RectangleShape maxHealthBar;
    sf::RectangleShape currentHealthBar;

    void doLoad() override;
    void doDraw(sf::RenderWindow &) override;
    bool doTakeDamage(const int &) override;

public:
    friend std::ostream &operator<<(std::ostream &, const Enemy &);

    std::vector<Projectile> update(const sf::Vector2f &);
    static Enemy spawnEnemy(sf::Texture &, const sf::Vector2f &, float, const int &, sf::Texture &);
};