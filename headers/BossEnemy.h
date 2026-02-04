#pragma once

#include "../headers/Enemy.h"

class BossEnemy : public Enemy
{
    std::unique_ptr<Weapon> cannon;
    std::unique_ptr<Weapon> laser;
    std::unique_ptr<Weapon> antiCampLaser;
    sf::Clock antiCampClock;
    sf::Angle offset;

    std::vector<std::unique_ptr<Attack>> doUpdate(const float &, const sf::Vector2f &, const RoomElements &, const std::vector<std::unique_ptr<Enemy>> &, int[GRID_SIZE_X][GRID_SIZE_Y]) override;
    void doLoad() override;

    bool checkLineOfSight(const sf::Vector2f &, const std::vector<std::unique_ptr<Object>> &) const;

public:
    BossEnemy(const sf::Vector2f &, float, const int &);
};

