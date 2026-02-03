#pragma once

#include "../headers/Entity.h"
#include "../headers/Gun.h"
#include "../headers/Sword.h"
#include "../headers/Spear.h"
#include "../headers/Laser.h"
#include "../headers/Door.h"

#include <nlohmann/json.hpp>
#include <queue>
#include <fstream>

struct RoomElements
{
    RoomElements(
        const std::vector<std::unique_ptr<Object>> &o,
        const std::vector<std::unique_ptr<Collider>> &w,
        const std::vector<std::unique_ptr<Door>> &d)
        : obstacles(o), walls(w), doors(d)
    {
    }

    const std::vector<std::unique_ptr<Object>> &obstacles;
    const std::vector<std::unique_ptr<Collider>> &walls;
    const std::vector<std::unique_ptr<Door>> &doors;
};

class Enemy : public Entity
{
protected:
    std::unique_ptr<Weapon> weapon;
    sf::RectangleShape maxHealthBar;
    sf::RectangleShape currentHealthBar;
    sf::Vector2i gridPosition;
    sf::Vector2f target;
    sf::Clock updateClock;

    void doDraw(sf::RenderWindow &) const override;
    bool doTakeDamage(const int &) override;
    virtual std::vector<std::unique_ptr<Attack>> doUpdate(const float &, const sf::Vector2f &, const RoomElements &, const std::vector<std::unique_ptr<Enemy>> &, int[GRID_SIZE_X][GRID_SIZE_Y]) = 0;
    virtual void doLoad();
    
    void enemyMove(const float &, const RoomElements &, const std::vector<std::unique_ptr<Enemy>> &, const sf::Angle &);

public:
    virtual ~Enemy() = default;
    Enemy(const sf::Vector2f &, float, const int &, const sf::Texture &);
    friend std::ostream &operator<<(std::ostream &, const Enemy &);

    void load();
    std::vector<std::unique_ptr<Attack>> update(const float &, const sf::Vector2f &, const RoomElements &, const std::vector<std::unique_ptr<Enemy>> &, int[GRID_SIZE_X][GRID_SIZE_Y]);
    sf::Vector2f getPosition() const;
};