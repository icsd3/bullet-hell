#include "../headers/EnemyRoom.h"

EnemyRoom::EnemyRoom(const int count, Player &player)
    : Room(player), nrOfEnemies(count)
{
    obstacleTexture = &ResourceManager::getTexture(TextureType::Obstacle);
    animationClock.reset();
}

void EnemyRoom::doLoad(std::weak_ptr<Room> u, std::weak_ptr<Room> r, std::weak_ptr<Room> d, std::weak_ptr<Room> l)
{
    Room::doLoad(u, r, d, l);

    std::mt19937 &rng = Utils::getRng();
    std::uniform_int_distribution<int> nrOfObstaclesDist(5, 10);
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
                obstacles.push_back(std::make_unique<Object>(sf::Vector2f(180.f + x * 120.f, 180.f + y * 120.f), *obstacleTexture));
                obstacles.back()->load({114.f, 0.f}, {0.5f, 0.5f}, {1.f, 0.9f}, {0.5f, 0.5f}, {0.f, 0.f});
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
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);
    sf::Sprite drawBg = backgroundSprite;
    drawBg.setPosition(Utils::mapToScreen(backgroundSprite.getPosition(), window));
    drawBg.scale(scaleFactor);
    window.draw(drawBg);

    for (auto &door : doors)
        door->draw(window);

    for (auto &wall : walls)
        wall->draw(window);

    for (auto &obstacle : obstacles)
        obstacle->draw(window);

    for (auto &enemy : enemies)
        enemy->draw(window);

    player.draw(window);

    for (auto &projectile : playerAttacks)
        projectile->draw(window);

    for (auto &projectile : enemyAttacks)
        projectile->draw(window);
}

std::pair<int, std::weak_ptr<Room>> EnemyRoom::doUpdate(const float &dt)
{
    bool finished = enemies.empty();

    if (finished)
        animationClock.start();

    std::pair<int, std::weak_ptr<Room>> action = Room::doUpdate(dt);

    for (size_t i = 0; i < playerAttacks.size();)
    {
        if (checkEnemyHits(*playerAttacks[i])  && playerAttacks[i]->takeDamage(1))
            playerAttacks.erase(playerAttacks.begin() + i);

        else
            i++;
    }

    for (int i = 0; i < 14; i++)
        for (int j = 0; j < 7; j++)
            if (grid[i][j] == 2)
                grid[i][j] = 0;

    for (auto &enemy : enemies)
    {
        std::vector<std::unique_ptr<Attack>> attacks = enemy->update(dt, player.getPosition(), RoomElements(obstacles, walls, doors), enemies, grid);

        for (auto &bullet : attacks)
        {
            enemyAttacks.push_back(std::move(bullet));
            enemyAttacks.back()->load();
        }
    }

    for (size_t i = 0; i < enemyAttacks.size();)
    {
        if (enemyAttacks[i]->update(dt))
            enemyAttacks.erase(enemyAttacks.begin() + i);

        else
        {
            if ((checkPlayerHits(*enemyAttacks[i]) && enemyAttacks[i]->takeDamage(1)) || checkEntityCollisions(*enemyAttacks[i]))
                enemyAttacks.erase(enemyAttacks.begin() + i);

            else
                i++;
        }
    }

    if (action.first >= 0)
        enemyAttacks.clear();

    return action;
}

bool EnemyRoom::checkEnemyHits(const Attack &attack)
{
    for (size_t i = 0; i < enemies.size();)
    {
        if (int damage = attack.hits(*enemies[i]))
        {
            if (enemies[i]->takeDamage(damage))
                enemies.erase(enemies.begin() + i);
            return true;
        }

        else
            i++;
    }
    return false;
}

bool EnemyRoom::checkPlayerHits(const Attack &attack)
{
    if (int damage = attack.hits(player))
    {
        if (player.takeDamage(damage))
            std::cout << "game over\n";
        return true;
    }
    return false;
}

void EnemyRoom::doStart()
{
    if (!isOpen)
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
                if (grid[x][y] == 0 && (grid[x + 1][y] != 1 || grid[x - 1][y] != 1 || grid[x][y + 1] != 1 || grid[x][y - 1] != 1))
                {
                    grid[x][y] = 2;
                    enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(180.f + x * 120.f, 180.f + y * 120.f), 100, 100, false));
                    enemies.back()->load();
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
        if (player.collidesWith(*obstacle))
            collides = -2;

    return collides;
}

bool EnemyRoom::doCheckEntityCollisions(const Entity &entity)
{
    bool collides = Room::doCheckEntityCollisions(entity);

    for (const auto &obstacle : obstacles)
        if (entity.collidesWith(*obstacle))
            collides = true;

    return collides;
}

void EnemyRoom::printDetails(std::ostream &os) const
{
    os << "Enemy room:\n";
    Room::printDetails(os);
    os << "    Enemies:\n";
    os << "        Count: " << enemies.size() << "\n";
    if (!enemies.empty())
        for (size_t i = 0; i < enemies.size(); i++)
            os << "        Enemy " << i + 1 << ":\n            " << *enemies[i] << "\n\n";
    os << "    Enemy Projectiles:\n";
    os << "        Count: " << enemyAttacks.size() << "\n";
    if (!enemyAttacks.empty())
        for (size_t i = 0; i < enemyAttacks.size(); i++)
            os << "        Projectile " << i + 1 << ":\n            " << enemyAttacks[i] << "\n\n";
}