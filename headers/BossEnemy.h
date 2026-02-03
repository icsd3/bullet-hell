#pragma once

#include "../headers/Enemy.h"

class BossEnemy : public Enemy
{
    std::unique_ptr<Weapon> cannon;
    std::unique_ptr<Weapon> laser;
    sf::Angle offset;

    std::vector<std::unique_ptr<Attack>> doUpdate(const float &, const sf::Vector2f &, const RoomElements &, const std::vector<std::unique_ptr<Enemy>> &, int[GRID_SIZE_X][GRID_SIZE_Y]) override;
    void doLoad() override;

public:
    BossEnemy(const sf::Vector2f &, float, const int &);
};

