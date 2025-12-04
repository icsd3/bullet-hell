#include "../headers/EnemyRoom.h"

EnemyRoom::EnemyRoom(const sf::Texture &dv, const sf::Texture &dh, const sf::Texture &background, sf::Texture &et, sf::Texture &ept)
    :Room(dv, dh, background), enemyTexture(&et), enemyProjectileTexture(&ept)
{
    animationClock.reset();
}

void EnemyRoom::doLoad(std::weak_ptr<Room> u, std::weak_ptr<Room> r, std::weak_ptr<Room> d, std::weak_ptr<Room> l)
{
    Room::doLoad(u, r, d, l);

    std::mt19937 &rng = Utils::getRng();
    std::uniform_int_distribution<int> nrOfObstaclesDist(3, 6);
    std::uniform_int_distribution<int> xDist(1, 25);
    std::uniform_int_distribution<int> yDist(1, 11);
    int nrOfObstacles = nrOfObstaclesDist(rng);

    for (int i = 0; i < nrOfObstacles; i++)
    {
        int x = xDist(rng);;
        int y = yDist(rng);
        while (grid[x][y] == 0)
        {
            if (grid[x-1][y-1] == 0 && grid[x-1][y] == 0 && grid[x-1][y+1] == 0 &&
                grid[x][y-1] == 0 && grid[x][y+1] == 0 &&
                grid[x+1][y-1] == 0 && grid[x+1][y] == 0 && grid[x+1][y+1] == 0)
            {
                grid[x][y] = 1;
                Object obstacle(sf::Vector2f(60.f + x * 60.f, 60.f + y * 60.f), false, sf::Vector2f(60.f, 60.f));
                obstacles.push_back(obstacle);
                obstacles.back().load();
                break;
            }
            x = xDist(rng);;
            y = yDist(rng);
        }
    }
}

void EnemyRoom::doDraw(sf::RenderWindow &window)
{
    Room::doDraw(window);

    for (auto &projectile : enemyProjectiles)
        projectile.draw(window);

    for (auto &enemy : enemies)
        enemy.draw(window);
}

std::pair<int, std::weak_ptr<Room>> EnemyRoom::doUpdate(const float &dt)
{
    bool finished = enemies.empty();

    if (finished)
        animationClock.start();

    std::pair<int, std::weak_ptr<Room>> action = Room::doUpdate(dt);

    for (size_t i = 0; i < playerProjectiles.size();)
    {
        if (checkEnemyHits(playerProjectiles[i]))
            playerProjectiles.erase(playerProjectiles.begin() + i);

        else
            i++;
    } 

    sf::Vector2f enemyTarget = player.getPosition();

    for(auto &enemy : enemies)
    {
        std::vector<Projectile> bullets = enemy.update(enemyTarget);

        for(const auto &bullet : bullets)
        {
            enemyProjectiles.push_back(bullet);
            enemyProjectiles.back().load();
        }
    }

    for (size_t i = 0; i < enemyProjectiles.size();)
    {
        if (enemyProjectiles[i].update(dt))
            enemyProjectiles.erase(enemyProjectiles.begin() + i);

        else
        {
            if (checkPlayerHits(enemyProjectiles[i]) || checkEntityCollisions(enemyProjectiles[i]))
                enemyProjectiles.erase(enemyProjectiles.begin() + i);

            else
                i++;
        }
    }

    if (action.first >= 0)
        enemyProjectiles.clear();

    return action;
}

bool EnemyRoom::checkEnemyHits(const Projectile &projectile)
{
    for (size_t i = 0; i < enemies.size();)
    {
        if (int damage = projectile.hits(enemies[i]))
        {
            if (enemies[i].takeDamage(damage))
                enemies.erase(enemies.begin() + i);
            return true;
        }

        else
            i++;
    }
    return false;
}

bool EnemyRoom::checkPlayerHits(const Projectile &projectile)
{
    if (int damage = projectile.hits(player))
    {
        if (player.takeDamage(damage))
            std::cout<<"game over\n";
        return true;
    }
    return false;
}

void EnemyRoom::doStart()
{
    if (!open)
        for (int i = 1; i <= 4; i++)
        {
            enemies.push_back(Enemy::spawnEnemy(*enemyTexture, sf::Vector2f(300.f + i%2 * 1320.f, 300.f + (i-1) / 2 * 480.f), 100.f, 100, *enemyProjectileTexture));
            enemies.back().load();
        }
}

int EnemyRoom::doCheckPlayerCollisions()
{
    int collides = Room::doCheckPlayerCollisions();

    // for (const auto &obstacle : obstacles)
    //     if(entity.collidesWith(obstacle))
    //         collides = -2;
    
    return collides;
}

bool EnemyRoom::doCheckEntityCollisions(const Entity &entity)
{
    bool collides = Room::doCheckEntityCollisions(entity);

    // for (const auto &obstacle : obstacles)
    //     if(entity.collidesWith(obstacle))
    //         collides = true;

    return collides;
}

std::ostream &operator<<(std::ostream &os, const EnemyRoom &room)
{
    os << "Enemy room:\n";
    os << static_cast<const Room &>(room);
    os << "    Enemies:\n";
    os << "        Count: " << room.enemies.size() << "\n";
    if (!room.enemies.empty())
        for (size_t i = 0; i < room.enemies.size(); i++)
            os << "        Enemy " << i + 1 << ":\n            " << room.enemies[i] << "\n\n";
    os << "    Enemy Projectiles:\n";
    os << "        Count: " << room.enemyProjectiles.size() << "\n";
    if (!room.enemyProjectiles.empty())
        for (size_t i = 0; i < room.enemyProjectiles.size(); i++)
            os << "        Projectile " << i + 1 << ":\n            " << room.enemyProjectiles[i] << "\n\n";
    return os;
}