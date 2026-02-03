#pragma once

#include "../headers/Enemy.h"

class LaserEnemy : public Enemy
{
    std::vector<std::unique_ptr<Attack>> doUpdate(const float &, const sf::Vector2f &, const RoomElements &, const std::vector<std::unique_ptr<Enemy>> &, int[GRID_SIZE_X][GRID_SIZE_Y]) override;

public:
    LaserEnemy(const sf::Vector2f &, float, const int &);
};