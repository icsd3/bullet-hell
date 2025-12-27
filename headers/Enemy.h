#pragma once

#include "../headers/Entity.h"
#include "../headers/Gun.h"
#include "../headers/Door.h"
#include "../headers/ResourceManager.h"

#include <nlohmann/json.hpp>
#include <queue>
#include <fstream>

class Enemy : public Entity
{
    std::unique_ptr<Weapon> weapon;
    sf::RectangleShape maxHealthBar;
    sf::RectangleShape currentHealthBar;
    sf::Vector2i gridPosition;
    sf::Vector2f target;
    sf::Clock updateClock;

    void doDraw(sf::RenderWindow &) const override;
    bool doTakeDamage(const int &) override;
    virtual bool checkLineOfSight(const sf::Vector2f &, const sf::Vector2f &, const std::vector<Object> &) const;
    void enemyMove(const float &, const std::vector<Object> &, const std::vector<Collider> &, const std::vector<Door> &, const std::vector<std::unique_ptr<Enemy>> &);
    sf::Vector2f nextPathPoint(const sf::Vector2i &start, const sf::Vector2i &goal, const int grid[14][7]);

public:
    Enemy(const sf::Vector2f &, float, const int &, const bool &);
    friend std::ostream &operator<<(std::ostream &, const Enemy &);

    void load();
    std::vector<Projectile> update(const float &, const sf::Vector2f &, const std::vector<Object> &, const std::vector<Collider> &, const std::vector<Door> &, const std::vector<std::unique_ptr<Enemy>> &, int[14][7]);
};