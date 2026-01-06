#include "../headers/BossRoom.h"

BossRoom::BossRoom(Player &player)
    : EnemyRoom(1, player)
{
}

void BossRoom::doLoad(std::weak_ptr<Room> u, std::weak_ptr<Room> r, std::weak_ptr<Room> d, std::weak_ptr<Room> l)
{
    Room::doLoad(u, r, d, l);

    std::mt19937 &rng = Utils::getRng();
    std::uniform_int_distribution<int> nrOfObstaclesDist(4, 8);
    std::uniform_int_distribution<int> xDist(1, 12);
    std::uniform_int_distribution<int> yDist(1, 5);
    int nrOfObstacles = nrOfObstaclesDist(rng);

    for (int i = 1; i <= nrOfObstacles; i++)
    {
        int x = xDist(rng);
        int y = yDist(rng);
        while (true)
        {
            if ((x < 4 || x > 9) && (y < 2 || y > 4) && grid[x][y] == 0)
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

void BossRoom::doDraw(sf::RenderWindow &window)
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

std::pair<int, std::weak_ptr<Room>> BossRoom::doUpdate(const float &dt)
{
    bool finished = enemies.empty();

    if (finished)
        animationClock.start();

    std::pair<int, std::weak_ptr<Room>> action = Room::doUpdate(dt);

    for (size_t i = 0; i < playerAttacks.size();)
    {
        if (checkEnemyHits(*playerAttacks[i]))
            playerAttacks.erase(playerAttacks.begin() + i);

        else
            i++;
    }

    for (int i = 0; i < 14; i++)
        for (int j = 0; j < 7; j++)
            if (grid[i][j] == 2)
                grid[i][j] = 0;

    if (!enemies.empty())
    {
        std::vector<std::unique_ptr<Attack>> attacks = enemies.front()->update(dt, player.getPosition(), RoomElements(obstacles, walls, doors), enemies, grid);

        for (auto &bullet : attacks)
        {
            enemyAttacks.push_back(std::move(std::move(bullet)));
            enemyAttacks.back()->load();
        }
    }

    for (size_t i = 0; i < enemyAttacks.size();)
    {
        if (enemyAttacks[i]->update(dt))
            enemyAttacks.erase(enemyAttacks.begin() + i);

        else
        {
            if (checkPlayerHits(*enemyAttacks[i]) || checkEntityCollisions(*enemyAttacks[i]))
                enemyAttacks.erase(enemyAttacks.begin() + i);

            else
                i++;
        }
    }

    if (action.first >= 0)
        enemyAttacks.clear();

    return action;
}

bool BossRoom::checkEnemyHits(const Attack &attack)
{
    if (!enemies.empty())
    {
        if (int damage = attack.hits(*enemies.front()))
        {
            if (enemies.front()->takeDamage(damage))
                enemies.clear();
            return true;
        }
    }
    return false;
}

void BossRoom::doStart()
{
    if (!open)
    {
        std::mt19937 &rng = Utils::getRng();

        std::uniform_int_distribution<int> xDist(4, 9);
        std::uniform_int_distribution<int> yDist(2, 4);

        int x = xDist(rng);
        int y = yDist(rng);

        grid[x][y] = 2;
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(180.f + x * 120.f, 180.f + y * 120.f), 100, 1000, true));
        enemies.front()->load();
    }
}

bool BossRoom::isCleared() const
{
    return enemies.empty();
}

void BossRoom::printDetails(std::ostream &os) const
{
    os << "Boss room:\n";
    Room::printDetails(os);
    if (!enemies.empty())
    {
        os << "    Boss:\n";
        os << "        " << *enemies.front() << "\n\n";
    }
    else
        os << "    Boss: defeated\n\n";
    os << "    Boss Projectiles:\n";
    os << "        Count: " << enemyAttacks.size() << "\n";
    if (!enemyAttacks.empty())
        for (size_t i = 0; i < enemyAttacks.size(); i++)
            os << "        Projectile " << i + 1 << ":\n            " << enemyAttacks[i] << "\n\n";
}