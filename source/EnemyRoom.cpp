#include "../headers/EnemyRoom.h"

EnemyRoom::EnemyRoom(const sf::Texture &dv, const sf::Texture &dh, const sf::Texture &background, sf::Texture &et, sf::Texture &ept, sf::Texture &ot, const int ne)
    :Room(dv, dh, background), enemyTexture(&et), enemyProjectileTexture(&ept), obstacleTexture(&ot), nrOfEnemies(ne)
{
    animationClock.reset();
}

void EnemyRoom::doLoad(std::weak_ptr<Room> u, std::weak_ptr<Room> r, std::weak_ptr<Room> d, std::weak_ptr<Room> l)
{
    Room::doLoad(u, r, d, l);

    std::mt19937 &rng = Utils::getRng();
    std::uniform_int_distribution<int> nrOfObstaclesDist(30,32);
    std::uniform_int_distribution<int> xDist(1, 12);
    std::uniform_int_distribution<int> yDist(1, 5);
    int nrOfObstacles = nrOfObstaclesDist(rng);

    for (int i = 1; i <= nrOfObstacles; i++)
    {
        int x = xDist(rng);
        int y = yDist(rng);
        while (true)
        {
            if (grid[x][y] == 0)
            {
                grid[x][y] = 1;
                Object obstacle(sf::Vector2f(180.f + x * 120.f, 180.f + y * 120.f), false, *obstacleTexture);
                obstacles.push_back(obstacle);
                obstacles.back().load();
                break;
            }
            else
            {
                x = xDist(rng);
                y = yDist(rng);
            }
        }
    }
}

void EnemyRoom::doDraw(sf::RenderWindow &window)
{
    Room::doDraw(window);

    for (auto &obstacle : obstacles)
        obstacle.draw(window);

    for (auto &enemy : enemies)
        enemy.draw(window);

    for (auto &projectile : enemyProjectiles)
        projectile.draw(window);
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

    for (int i = 0; i < 14; i++)
        for (int j = 0; j < 7; j++)
            if (grid[i][j] == 2)
                grid[i][j] = 0; 

    for(auto &enemy : enemies)
    {
        std::vector<Projectile> bullets = enemy.update(dt, player.getPosition(), obstacles, walls, doors, enemies, grid);

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
    {
        std::mt19937 &rng = Utils::getRng();

        std::uniform_int_distribution<int> xDist;
        std::uniform_int_distribution<int> yDist;

        if (player.getPosition().x < 300.f)
        {
            xDist = std::uniform_int_distribution<int>(7, 12);
            yDist = std::uniform_int_distribution<int>(1, 5);
        }
        else if (player.getPosition().x > LOGICAL_WIDTH - 300.f)
        {
            xDist = std::uniform_int_distribution<int>(1, 6);
            yDist = std::uniform_int_distribution<int>(1, 5);
        }
        else if (player.getPosition().y < 300.f)
        {
            xDist = std::uniform_int_distribution<int>(1, 12);
            yDist = std::uniform_int_distribution<int>(4, 5);
        }
        else
        {
            xDist = std::uniform_int_distribution<int>(1, 12);
            yDist = std::uniform_int_distribution<int>(1, 2);
        }


        for (int i = 0; i < nrOfEnemies; i++)
        {
            int x = xDist(rng);
            int y = yDist(rng);
            while (true)
            {
                if (grid[x][y] == 0 && (grid[x+1][y] != 1 || grid[x-1][y] != 1 || grid[x][y+1] != 1 || grid[x][y-1] != 1))
                {
                    grid[x][y] = 2;
                    enemies.push_back(Enemy::spawnEnemy(*enemyTexture, sf::Vector2f(180.f + x * 120.f, 180.f + y * 120.f), 0.05f, 100, *enemyProjectileTexture));
                    enemies.back().load();
                    break;
                }
                else
                {
                    x = xDist(rng);
                    y = yDist(rng);
                }
            }
        }
    }
}

int EnemyRoom::doCheckPlayerCollisions()
{
    int collides = Room::doCheckPlayerCollisions();

    for (const auto &obstacle : obstacles)
        if(player.collidesWith(obstacle))
            collides = -2;
    
    return collides;
}

bool EnemyRoom::doCheckEntityCollisions(const Entity &entity)
{
    bool collides = Room::doCheckEntityCollisions(entity);

    for (const auto &obstacle : obstacles)
        if(entity.collidesWith(obstacle))
            collides = true;

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