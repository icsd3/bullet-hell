#pragma once

#include "../headers/Entities.h"
#include "../headers/Weapons.h"
#include "../headers/Door.h"

#include <queue>

class Enemy : public Entity
{
    Enemy(const sf::Vector2f &, const bool &, sf::Texture &, const float, const int &, const Weapon &);
    
    Weapon weapon;
    sf::RectangleShape maxHealthBar;
    sf::RectangleShape currentHealthBar;
    sf::Vector2i gridPosition;
    sf::Vector2f target;
    sf::Clock updateClock;

    void doLoad() override;
    void doDraw(sf::RenderWindow &) override;
    bool doTakeDamage(const int &) override;
    virtual bool checkLineOfSight(const sf::Vector2f &, const sf::Vector2f &, const std::vector<Object> &) const;
    void move(const float &, const std::vector<Object> &, const std::vector<Object> &, const std::vector<Door> &, const std::vector<Enemy> &);
    sf::Vector2f nextPathPoint(const sf::Vector2i& start, const sf::Vector2i& goal, const int grid[14][7]);

public:
    friend std::ostream &operator<<(std::ostream &, const Enemy &);

    std::vector<Projectile> update(const float &, const sf::Vector2f &, const std::vector<Object> &, const std::vector<Object> &, const std::vector<Door> &, const std::vector<Enemy> &, int[14][7]);
    static Enemy spawnEnemy(sf::Texture &, const sf::Vector2f &, float, const int &, sf::Texture &);
};