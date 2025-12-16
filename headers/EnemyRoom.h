#pragma once

#include "../headers/Room.h"

class EnemyRoom : public Room
{
    sf::Texture *enemyTexture;
    sf::Texture *enemyProjectileTexture;
    sf::Texture *obstacleTexture;

    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<Projectile> enemyProjectiles;

    std::vector<Object> obstacles;

    const int nrOfEnemies;

    void doLoad(std::weak_ptr<Room>, std::weak_ptr<Room>, std::weak_ptr<Room>, std::weak_ptr<Room>) override;
    void doStart() override;
    void doDraw(sf::RenderWindow &) override;
    std::pair<int, std::weak_ptr<Room>> doUpdate(const float &) override;
    int doCheckPlayerCollisions() override;
    bool doCheckEntityCollisions(const Entity &) override;

    bool checkEnemyHits(const Projectile &);
    bool checkPlayerHits(const Projectile &);

public:
    EnemyRoom(const sf::Texture &, const sf::Texture &, const sf::Texture &, sf::Texture &, sf::Texture &, sf::Texture &, const int, Player &);
    EnemyRoom(const EnemyRoom &) = delete;
    EnemyRoom &operator=(const EnemyRoom &) = delete;
    friend std::ostream &operator<<(std::ostream &, const EnemyRoom &);
};