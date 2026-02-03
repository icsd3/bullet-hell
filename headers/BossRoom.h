#pragma once

#include "../headers/EnemyRoom.h"
#include "../headers/BossEnemy.h"

class BossRoom : public EnemyRoom
{
    void doLoad(std::weak_ptr<Room>, std::weak_ptr<Room>, std::weak_ptr<Room>, std::weak_ptr<Room>) override;
    void doStart() override;
    void doDraw(sf::RenderWindow &) override;
    std::pair<int, std::weak_ptr<Room>> doUpdate(const float &) override;
    void printDetails(std::ostream &) const override;
    bool isCleared() const override;

    bool checkEnemyHits(const Attack &) override;

public:
    explicit BossRoom(Player &);
    BossRoom(const BossRoom &) = delete;
    BossRoom &operator=(const BossRoom &) = delete;
};