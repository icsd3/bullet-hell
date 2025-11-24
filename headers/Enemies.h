#pragma once

#include "../headers/Entities.h"
#include "../headers/Weapons.h"

class Enemy : public Entity
{
    Enemy(const bool &, const sf::Vector2f &, const bool &, const std::string &, const sf::Texture &, float, const int &, const Weapon &);
    int maxHealth;
    Weapon weapon;
    int currentHealth;

    sf::RectangleShape maxHealthBar;
    sf::RectangleShape currentHealthBar;

public:
    static Enemy spawnEnemy(const sf::Texture &, const sf::Vector2f &, float, const int &);

    void load(const sf::Texture &);
    void draw(sf::RenderWindow &);
    void update();
    bool takeDamage(const int &);
    std::vector<Projectile> fire(const sf::Vector2f &, const sf::Texture &);
    bool canFire();

    Enemy(const Enemy &) = default;
    Enemy &operator=(const Enemy &);
    ~Enemy() = default;
    friend std::ostream &operator<<(std::ostream &, const Enemy &);
};