#include "../headers/Enemy.h"

Enemy::Enemy(const sf::Vector2f &pos, float spd, const int &mh, const bool &boss)
    : Entity(pos, (boss == false) ? TextureManager::getTexture(TextureType::Enemy) : TextureManager::getTexture(TextureType::Boss), spd, mh), gridPosition(0, 0), target(pos)
{
    std::ifstream file("json/Guns.json");
    nlohmann::json data;
    file >> data;
    const auto &w = data[0];
    int type = (boss == false) ? 2 : 3;
    weapon = std::make_unique<Gun>(
        w["name"],
        w["damage"],
        w["fire_rate"],
        0.3f,
        w["bullet_nr"],
        w["spread_angle"],
        w["range"],
        w["bullet_speed"],
        type);
}

void Enemy::load()
{
    Entity::load(60.f, {0.6f, 0.6f}, {0.5f, 1.0f}, {0.f, 0.5f}, 6, {
        {4.5f / 14, 0.f}, 
        {9.5f / 14, 0.f}, 
        {1.f, 4.5f / 14}, 
        {1.f, 1.f}, 
        {0.f, 1.f}, 
        {0.f, 4.5f / 14}
    });

    maxHealthBar.setSize(sf::Vector2f(60.f, 12.f));
    maxHealthBar.setFillColor(sf::Color(75, 0, 0, 175));
    maxHealthBar.setOrigin(sf::Vector2f(maxHealthBar.getLocalBounds().size.x / 2.f, maxHealthBar.getLocalBounds().size.y / 2.f));
    maxHealthBar.setPosition(sf::Vector2f(position.x, position.y - 60.f / 2.f - maxHealthBar.getSize().y));
    maxHealthBar.setOutlineThickness(2.f);
    maxHealthBar.setOutlineColor(sf::Color::Black);

    currentHealthBar.setSize(maxHealthBar.getSize());
    currentHealthBar.setFillColor(sf::Color(150, 0, 0, 175));
    currentHealthBar.setOrigin(sf::Vector2f(0, currentHealthBar.getLocalBounds().size.y / 2.f));
    currentHealthBar.setPosition(sf::Vector2f(maxHealthBar.getPosition().x - maxHealthBar.getSize().x / 2.f, maxHealthBar.getPosition().y));

    int gridX = static_cast<int>((position.x - 120.f) / 120.f);
    int gridY = static_cast<int>((position.y - 120.f) / 120.f);
    gridPosition = {gridX, gridY};

    weapon->reset();
    updateClock.reset();
}

void Enemy::doDraw(sf::RenderWindow &window) const
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    Entity::doDraw(window);

    sf::RectangleShape drawMaxHealthBar = maxHealthBar;
    drawMaxHealthBar.setPosition(Utils::mapToScreen(maxHealthBar.getPosition(), window));
    drawMaxHealthBar.setSize(sf::Vector2f(maxHealthBar.getSize().x * scaleFactor.x, maxHealthBar.getSize().y * scaleFactor.y));
    drawMaxHealthBar.setOrigin(sf::Vector2f(drawMaxHealthBar.getSize().x / 2.f, drawMaxHealthBar.getSize().y / 2.f));

    drawMaxHealthBar.setOutlineThickness(maxHealthBar.getOutlineThickness() * scaleFactor.x);

    window.draw(drawMaxHealthBar);

    sf::RectangleShape drawCurrentHealthBar = currentHealthBar;
    drawCurrentHealthBar.setPosition(Utils::mapToScreen(currentHealthBar.getPosition(), window));
    drawCurrentHealthBar.setSize(sf::Vector2f(currentHealthBar.getSize().x * scaleFactor.x, currentHealthBar.getSize().y * scaleFactor.y));
    drawCurrentHealthBar.setOrigin(sf::Vector2f(0, drawCurrentHealthBar.getSize().y / 2.f));

    window.draw(drawCurrentHealthBar);
}

std::vector<Projectile> Enemy::update(const float &dt, const sf::Vector2f &playerPosition, const std::vector<Object> &obstacles, const std::vector<Collider> &walls, const std::vector<Door> &doors, const std::vector<std::unique_ptr<Enemy>> &enemies, int grid[14][7])
{
    sf::Vector2i playerGridPosition(static_cast<int>((playerPosition.x - 120.f) / 120.f), static_cast<int>((playerPosition.y - 120.f) / 120.f));

    weapon->update();

    std::vector<Projectile> bullets;

    if (checkLineOfSight(position, playerPosition, obstacles))
    {
        bullets = weapon->fire(position, playerPosition);
        updateClock.start();
        if (updateClock.getElapsedTime().asSeconds() > 0.5f)
        {
            target = position;
            updateClock.reset();
        }
    }

    else
    {
        target = nextPathPoint(gridPosition, playerGridPosition, grid);
        updateClock.reset();
    }

    enemyMove(dt, obstacles, walls, doors, enemies);

    grid[gridPosition.x][gridPosition.y] = 2;

    int tGridX = static_cast<int>((target.x - 120.f) / 120.f);
    int tGridY = static_cast<int>((target.y - 120.f) / 120.f);

    if (tGridX >= 0 && tGridX < 14 && tGridY >= 0 && tGridY < 7)
    {
        if (tGridX != gridPosition.x || tGridY != gridPosition.y)
            grid[tGridX][tGridY] = 2;
    }

    return bullets;
}

sf::Vector2f Enemy::nextPathPoint(const sf::Vector2i &start, const sf::Vector2i &goal, const int grid[14][7])
{
    struct Node
    {
        int x, y;
        float g, h;
        Node *parent;
    };

    auto inBounds = [&](int x, int y)
    { return x >= 0 && x < 14 && y >= 0 && y < 7; };

    auto heuristic = [](int x1, int y1, int x2, int y2)
    {
        float dx = static_cast<float>(x2 - x1);
        float dy = static_cast<float>(y2 - y1);
        return std::sqrt(dx * dx + dy * dy);
    };

    const std::pair<sf::Vector2i, float> directions[8] = {
        {{1, 0}, 1.f}, {{-1, 0}, 1.f}, {{0, 1}, 1.f}, {{0, -1}, 1.f}, {{1, 1}, 1.4142f}, {{1, -1}, 1.4142f}, {{-1, 1}, 1.4142f}, {{-1, -1}, 1.4142f}};

    bool closed[7][14] = {{false}};
    std::unique_ptr<Node> nodes[7][14];

    auto cmp = [](const Node *a, const Node *b)
    {
        return a->g + a->h > b->g + b->h;
    };

    std::priority_queue<Node *, std::vector<Node *>, decltype(cmp)> open(cmp);

    nodes[start.y][start.x] = std::make_unique<Node>(Node{start.x, start.y, 0.f, heuristic(start.x, start.y, goal.x, goal.y), nullptr});
    open.push(nodes[start.y][start.x].get());

    while (!open.empty())
    {
        Node *current = open.top();
        open.pop();

        if (current->x == goal.x && current->y == goal.y)
        {
            Node *step = current;
            while (step->parent && (step->parent->x != start.x || step->parent->y != start.y))
                step = step->parent;
            return sf::Vector2f(180.f + step->x * 120.f, 180.f + step->y * 120.f);
        }

        closed[current->y][current->x] = true;

        for (auto &[dir, cost] : directions)
        {
            int nx = current->x + dir.x;
            int ny = current->y + dir.y;

            if (!inBounds(nx, ny) || grid[nx][ny] == 1 || closed[ny][nx])
                continue;

            if (dir.x != 0 && dir.y != 0)
                if (grid[current->x][current->y + dir.y] == 1 || grid[current->x + dir.x][current->y] == 1)
                    continue;

            float penalty = (grid[nx][ny] >= 2) ? 10.0f : 1.0f;
            float newG = current->g + (cost * penalty);

            if (!nodes[ny][nx])
            {
                nodes[ny][nx] = std::make_unique<Node>(Node{nx, ny, newG, heuristic(nx, ny, goal.x, goal.y), current});
                open.push(nodes[ny][nx].get());
            }
            else if (newG < nodes[ny][nx]->g)
            {
                nodes[ny][nx]->g = newG;
                nodes[ny][nx]->parent = current;
            }
        }
    }

    return sf::Vector2f(180.f + start.x * 120.f, 180.f + start.y * 120.f);
}

void Enemy::enemyMove(const float &dt, const std::vector<Object> &obstacles, const std::vector<Collider> &walls, const std::vector<Door> &doors, const std::vector<std::unique_ptr<Enemy>> &enemies)
{
    sf::Vector2f separation(0.f, 0.f);
    int neighbors = 0;

    for (const auto &enemy : enemies)
    {
        if (enemy.get() == this)
            continue;

        sf::Vector2f dir = position - enemy->position;
        float dist = dir.x * dir.x + dir.y * dir.y;

        if (dist < 3000.f && dist > 0.1f)
        {
            separation += dir / std::sqrt(dist);
            neighbors++;
        }
    }

    sf::Vector2f finalDir(0.f, 0.f);
    sf::Vector2f dir = target - position;
    float distToTarget = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (distToTarget > 30.0f)
    {
        finalDir = dir / distToTarget;
    }

    if (neighbors > 0)
    {
        finalDir += separation;
    }

    float finalLen = std::sqrt(finalDir.x * finalDir.x + finalDir.y * finalDir.y);

    if (finalLen > 0.f)
    {
        finalDir /= finalLen;

        sf::Vector2f movement = finalDir * speed * dt;

        auto checkCollision = [&]() -> bool
        {
            for (const auto &wall : walls)
                if (collidesWith(wall))
                    return true;
            for (const auto &door : doors)
                if (collidesWith(door))
                    return true;
            for (const auto &obstacle : obstacles)
                if (collidesWith(obstacle))
                    return true;
            for (const auto &enemy : enemies)
                if (enemy.get() != this && collidesWith(*enemy))
                    return true;
            return false;
        };

        auto performMove = [&](const sf::Vector2f &moveVec)
        {
            transform(moveVec, 0.2f, sf::Angle(sf::degrees(0.f)));
            maxHealthBar.move(moveVec);
            currentHealthBar.move(moveVec);
        };

        performMove(movement);

        if (checkCollision())
        {
            performMove(-movement);

            sf::Vector2f moveX(movement.x, 0.f);
            performMove(moveX);

            if (checkCollision())
            {
                performMove(-moveX);

                sf::Vector2f moveY(0.f, movement.y);
                performMove(moveY);

                if (checkCollision())
                {
                    performMove(-moveY);
                }
            }
        }
    }

    else
        target = position;

    int gridX = static_cast<int>((position.x - 120.f) / 120.f);
    int gridY = static_cast<int>((position.y - 120.f) / 120.f);
    gridPosition = {gridX, gridY};
}

bool Enemy::doTakeDamage(const int &dmg)
{
    currentHealth -= dmg;
    currentHealthBar.setSize(sf::Vector2f(1.f * currentHealth / maxHealth * maxHealthBar.getSize().x, currentHealthBar.getSize().y));

    if (currentHealth <= 0)
    {
        currentHealth = 0;
        return true;
    }
    return false;
}

bool Enemy::checkLineOfSight(const sf::Vector2f &origin, const sf::Vector2f &playerPosition, const std::vector<Object> &obstacles) const
{
    sf::ConvexShape lineOfSight;
    sf::Vector2f direction = playerPosition - origin;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= distance;
    lineOfSight.setPointCount(6);
    lineOfSight.setPoint(0, origin);
    lineOfSight.setPoint(1, origin + 5.f * (direction + direction.rotatedBy(sf::degrees(-90.f))));
    lineOfSight.setPoint(2, playerPosition - 5.f * (direction + direction.rotatedBy(sf::degrees(90.f))));
    lineOfSight.setPoint(3, playerPosition);
    lineOfSight.setPoint(4, playerPosition - 5.f * (direction + direction.rotatedBy(sf::degrees(-90.f))));
    lineOfSight.setPoint(5, origin + 5.f * (direction + direction.rotatedBy(sf::degrees(90.f))));

    for (const auto &obstacle : obstacles)
    {
        if (obstacle.collidesWith(lineOfSight))
            return false;
    }
    return true;
}

std::ostream &operator<<(std::ostream &os, const Enemy &enemy)
{
    os << static_cast<const Entity &>(enemy);
    os << "\n";
    os << "        Enemy Weapon: " << *enemy.weapon;
    return os;
}
