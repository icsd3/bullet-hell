#pragma once

#include "../headers/Room.h"

class EnemyRoom : public Room
{
    sf::Texture *obstacleTexture;

    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Attack>> enemyAttacks;

    std::vector<std::unique_ptr<Object>> obstacles;

    const int nrOfEnemies;

    void doLoad(std::weak_ptr<Room>, std::weak_ptr<Room>, std::weak_ptr<Room>, std::weak_ptr<Room>) override;
    void doStart() override;
    void doDraw(sf::RenderWindow &) override;
    std::pair<int, std::weak_ptr<Room>> doUpdate(const float &) override;
    int doCheckPlayerCollisions() override;
    bool doCheckEntityCollisions(const Entity &) override;
    void printDetails(std::ostream &) const override;

    bool checkEnemyHits(const Attack &);
    bool checkPlayerHits(const Attack &);

public:
    EnemyRoom(const int, Player &);
    EnemyRoom(const EnemyRoom &) = delete;
    EnemyRoom &operator=(const EnemyRoom &) = delete;
};