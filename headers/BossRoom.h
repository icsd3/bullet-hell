#pragma once

#include "../headers/Room.h"

class BossRoom : public Room
{
    sf::Texture *obstacleTexture;

    std::unique_ptr<Enemy> boss;
    std::vector<std::unique_ptr<Attack>> bossProjectiles;

    std::vector<Object> obstacles;

    void doLoad(std::weak_ptr<Room>, std::weak_ptr<Room>, std::weak_ptr<Room>, std::weak_ptr<Room>) override;
    void doStart() override;
    void doDraw(sf::RenderWindow &) override;
    std::pair<int, std::weak_ptr<Room>> doUpdate(const float &) override;
    int doCheckPlayerCollisions() override;
    bool doCheckEntityCollisions(const Entity &) override;
    void printDetails(std::ostream &) const override;

    bool checkBossHits(const Attack &);
    bool checkPlayerHits(const Attack &);

public:
    explicit BossRoom(Player &);
    BossRoom(const BossRoom &) = delete;
    BossRoom &operator=(const BossRoom &) = delete;
};