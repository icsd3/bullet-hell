#pragma once

#include "../headers/Enemy.h"

class GunEnemy : public Enemy
{
    virtual bool checkLineOfSight(const sf::Vector2f &, const sf::Vector2f &, const std::vector<std::unique_ptr<Object>> &) const;
    std::vector<std::unique_ptr<Attack>> doUpdate(const float &, const sf::Vector2f &, const RoomElements &, const std::vector<std::unique_ptr<Enemy>> &, int[GRID_SIZE_X][GRID_SIZE_Y]) override;
    sf::Vector2f nextPathPoint(const sf::Vector2i &start, const sf::Vector2i &goal, const int [GRID_SIZE_X][GRID_SIZE_Y]);

public:
    GunEnemy(const sf::Vector2f &, float, const int &);
};