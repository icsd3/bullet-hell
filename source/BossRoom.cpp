#include "../headers/BossRoom.h"

BossRoom::BossRoom(Player &player)
    : Room(player)
{
    obstacleTexture = &TextureManager::getObstacleTexture();
    animationClock.reset();
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
                Object obstacle(sf::Vector2f(180.f + x * 120.f, 180.f + y * 120.f), *obstacleTexture);
                obstacles.push_back(obstacle);
                obstacles.back().load(114.f, {1.f, 1.f}, {0.5f, 0.5f}, {0.f, 0.f});
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
        door.draw(window);

    for (auto &wall : walls)
        wall.draw(window);

    for (auto &obstacle : obstacles)
        obstacle.draw(window);

    if (boss)
        boss->draw(window);

    player.draw(window);

    for (auto &projectile : playerProjectiles)
        projectile.draw(window);

    for (auto &projectile : bossProjectiles)
        projectile.draw(window);
}

std::pair<int, std::weak_ptr<Room>> BossRoom::doUpdate(const float &dt)
{
    bool finished = !boss;

    if (finished)
        animationClock.start();

    std::pair<int, std::weak_ptr<Room>> action = Room::doUpdate(dt);

    for (size_t i = 0; i < playerProjectiles.size();)
    {
        if (checkBossHits(playerProjectiles[i]))
            playerProjectiles.erase(playerProjectiles.begin() + i);

        else
            i++;
    }

    for (int i = 0; i < 14; i++)
        for (int j = 0; j < 7; j++)
            if (grid[i][j] == 2)
                grid[i][j] = 0;

    if (boss)
    {
        std::vector<Projectile> bullets = boss->update(dt, player.getPosition(), obstacles, walls, doors, {}, grid);

        for (const auto &bullet : bullets)
        {
            bossProjectiles.push_back(bullet);
            bossProjectiles.back().load();
        }
    }

    for (size_t i = 0; i < bossProjectiles.size();)
    {
        if (bossProjectiles[i].update(dt))
            bossProjectiles.erase(bossProjectiles.begin() + i);

        else
        {
            if (checkPlayerHits(bossProjectiles[i]) || checkEntityCollisions(bossProjectiles[i]))
                bossProjectiles.erase(bossProjectiles.begin() + i);

            else
                i++;
        }
    }

    if (action.first >= 0)
        bossProjectiles.clear();

    return action;
}

bool BossRoom::checkBossHits(const Projectile &projectile)
{
    if (boss)
    {
        if (int damage = projectile.hits(*boss))
        {
            if (boss->takeDamage(damage))
                boss.reset();
            return true;
        }
    }
    return false;
}

bool BossRoom::checkPlayerHits(const Projectile &projectile)
{
    if (int damage = projectile.hits(player))
    {
        if (player.takeDamage(damage))
            std::cout << "game over\n";
        return true;
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
        boss = std::make_unique<Enemy>(sf::Vector2f(180.f + x * 120.f, 180.f + y * 120.f), 100, 1000);
        boss->load();
    }
}

int BossRoom::doCheckPlayerCollisions()
{
    int collides = Room::doCheckPlayerCollisions();

    for (const auto &obstacle : obstacles)
        if (player.collidesWith(obstacle))
            collides = -2;

    return collides;
}

bool BossRoom::doCheckEntityCollisions(const Entity &entity)
{
    bool collides = Room::doCheckEntityCollisions(entity);

    for (const auto &obstacle : obstacles)
        if (entity.collidesWith(obstacle))
            collides = true;

    return collides;
}

std::ostream &operator<<(std::ostream &os, const BossRoom &room)
{
    os << "Boss room:\n";
    os << static_cast<const Room &>(room);
    if (room.boss)
    {
        os << "    Boss:\n";
        os << "        " << *(room.boss) << "\n\n";
    }
    else
        os << "    Boss: defeated\n\n";
    os << "    Boss Projectiles:\n";
    os << "        Count: " << room.bossProjectiles.size() << "\n";
    if (!room.bossProjectiles.empty())
        for (size_t i = 0; i < room.bossProjectiles.size(); i++)
            os << "        Projectile " << i + 1 << ":\n            " << room.bossProjectiles[i] << "\n\n";
    return os;
}