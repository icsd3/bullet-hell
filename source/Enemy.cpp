#include "../headers/Enemy.h"

Enemy::Enemy(const sf::Vector2f &pos, float spd, const int &mh, const bool &boss)
    : Entity(pos, (boss == false) ? ResourceManager::getTexture(TextureType::Enemy) : ResourceManager::getTexture(TextureType::Boss), spd, mh), gridPosition(0, 0), target(pos)
{
    std::mt19937 &rng = Utils::getRng();
    std::uniform_int_distribution<int> enemyGunIndexRange(0, 2);
    int enemyGunIndex = enemyGunIndexRange(rng);
    float fireRateMaxOffset = 0.3f;

    weapon = std::make_unique<Gun>("json/EnemyGuns.json", enemyGunIndex, fireRateMaxOffset, (boss == false) ? AttackTextureType::Enemy : AttackTextureType::Boss);
}

void Enemy::load()
{
    sf::Vector2f scaleFactor = {60.f, 0.f};
    sf::Vector2f spriteOriginFactor = {0.5f, 0.5f};
    sf::Vector2f collisionBoxSizeFactor = {0.6f, 0.6f}; // relative to sprite dimensions
    sf::Vector2f collisionBoxOriginFactor = {0.5f, 1.0f}; // relative to collision box
    sf::Vector2f collisionBoxPositionFactor = {0.f, 0.5f}; // relative to sprite
    int hitBoxPointCount = 6;
    std::vector<sf::Vector2f> hitBoxPointFactors = { // relative to sprite dimensions (0,0) is top-left, (1,1) is bottom-right
        {0.3f, 0.f}, 
        {0.7f, 0.f}, 
        {1.f, 0.3f}, 
        {1.f, 1.f}, 
        {0.f, 1.f}, 
        {0.f, 0.3f}
    };
    ObjectLoadParams params(scaleFactor, spriteOriginFactor, collisionBoxSizeFactor, collisionBoxOriginFactor, collisionBoxPositionFactor);

    Entity::load(params, hitBoxPointCount, hitBoxPointFactors);

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

    int gridX = static_cast<int>((position.x - GRID_CELL_SIZE) / GRID_CELL_SIZE);
    int gridY = static_cast<int>((position.y - GRID_CELL_SIZE) / GRID_CELL_SIZE);
    gridPosition = {gridX, gridY};

    weapon->load(position);
    weapon->reset();
    updateClock.reset();
}

void Enemy::doDraw(sf::RenderWindow &window) const
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    Entity::doDraw(window);

    weapon->draw(window);

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

std::vector<std::unique_ptr<Attack>> Enemy::update(const float &dt, const sf::Vector2f &playerPosition, const RoomElements &elements, const std::vector<std::unique_ptr<Enemy>> &enemies, int grid[GRID_SIZE_X][GRID_SIZE_Y])
{
    sf::Vector2i playerGridPosition(static_cast<int>((playerPosition.x - GRID_CELL_SIZE) / GRID_CELL_SIZE), static_cast<int>((playerPosition.y - GRID_CELL_SIZE) / GRID_CELL_SIZE));

    sf::Angle angle = (playerPosition - position).angle();

    weapon->update(position, angle);

    std::vector<std::unique_ptr<Attack>> attacks;

    if (checkLineOfSight(position, playerPosition, elements.obstacles))
    {
        attacks = weapon->attack(position, playerPosition);
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

    enemyMove(dt, elements, enemies, angle);

    grid[gridPosition.x][gridPosition.y] = 2;

    int tGridX = static_cast<int>((target.x - GRID_CELL_SIZE) / GRID_CELL_SIZE);
    int tGridY = static_cast<int>((target.y - GRID_CELL_SIZE) / GRID_CELL_SIZE);

    if (tGridX >= 0 && tGridX < GRID_SIZE_X && tGridY >= 0 && tGridY < GRID_SIZE_Y)
    {
        if (tGridX != gridPosition.x || tGridY != gridPosition.y)
            grid[tGridX][tGridY] = 2;
    }

    return attacks;
}

sf::Vector2f Enemy::nextPathPoint(const sf::Vector2i &start, const sf::Vector2i &goal, const int grid[GRID_SIZE_X][GRID_SIZE_Y])
{
    struct Node
    {
        int x, y;
        float g, h;
        Node *parent;
    };

    auto inBounds = [&](int x, int y)
    { return x >= 0 && x < GRID_SIZE_X && y >= 0 && y < GRID_SIZE_Y; };

    auto heuristic = [](int x1, int y1, int x2, int y2)
    {
        float dx = static_cast<float>(x2 - x1);
        float dy = static_cast<float>(y2 - y1);
        return std::sqrt(dx * dx + dy * dy);
    };

    const std::pair<sf::Vector2i, float> directions[8] = {
        {{1, 0}, 1.f}, {{-1, 0}, 1.f}, {{0, 1}, 1.f}, {{0, -1}, 1.f}, {{1, 1}, 1.4142f}, {{1, -1}, 1.4142f}, {{-1, 1}, 1.4142f}, {{-1, -1}, 1.4142f}};

    bool closed_set[GRID_SIZE_X][GRID_SIZE_Y] = {{false}};
    std::unique_ptr<Node> nodes[GRID_SIZE_X][GRID_SIZE_Y];

    auto cmp = [](const Node *a, const Node *b)
    {
        return a->g + a->h > b->g + b->h;
    };

    std::priority_queue<Node *, std::vector<Node *>, decltype(cmp)> open_set(cmp);

    nodes[start.x][start.y] = std::make_unique<Node>(Node{start.x, start.y, 0.f, heuristic(start.x, start.y, goal.x, goal.y), nullptr});
    open_set.push(nodes[start.x][start.y].get());

    while (!open_set.empty())
    {
        Node *current = open_set.top();
        open_set.pop();

        if (current->x == goal.x && current->y == goal.y)
        {
            Node *step = current;
            while (step->parent && (step->parent->x != start.x || step->parent->y != start.y))
                step = step->parent;
            return sf::Vector2f(BORDER_SIZE + step->x * GRID_CELL_SIZE, BORDER_SIZE + step->y * GRID_CELL_SIZE);
        }

        closed_set[current->x][current->y] = true;

        for (auto &[dir, cost] : directions)
        {
            int nx = current->x + dir.x;
            int ny = current->y + dir.y;

            if (!inBounds(nx, ny) || grid[nx][ny] == 1 || closed_set[nx][ny])
                continue;

            if (dir.x != 0 && dir.y != 0)
                if (grid[current->x][current->y + dir.y] == 1 || grid[current->x + dir.x][current->y] == 1)
                    continue;

            float penalty = (grid[nx][ny] >= 2) ? 10.0f : 1.0f;
            float newG = current->g + (cost * penalty);

            if (!nodes[nx][ny])
            {
                nodes[nx][ny] = std::make_unique<Node>(Node{nx, ny, newG, heuristic(nx, ny, goal.x, goal.y), current});
                open_set.push(nodes[nx][ny].get());
            }
            else if (newG < nodes[nx][ny]->g)
            {
                nodes[nx][ny]->g = newG;
                nodes[nx][ny]->parent = current;
            }
        }
    }

    return sf::Vector2f(BORDER_SIZE + start.x * GRID_CELL_SIZE, BORDER_SIZE + start.y * GRID_CELL_SIZE);
}

void Enemy::enemyMove(const float &dt, const RoomElements &elements, const std::vector<std::unique_ptr<Enemy>> &enemies, const sf::Angle &angle)
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
            for (const auto &wall : elements.walls)
                if (collidesWith(*wall))
                    return true;
            for (const auto &door : elements.doors)
                if (collidesWith(*door))
                    return true;
            for (const auto &obstacle : elements.obstacles)
                if (collidesWith(*obstacle))
                    return true;
            for (const auto &enemy : enemies)
                if (enemy.get() != this && collidesWith(*enemy))
                    return true;
            return false;
        };

        auto performMove = [&](const sf::Vector2f &moveVec, const bool &facingLeft)
        {
            transform(moveVec, facingLeft, sf::Angle(sf::degrees(0.f)));
            maxHealthBar.move(moveVec);
            currentHealthBar.move(moveVec);
        };

        bool facingLeft = true;

        if (angle > sf::degrees(90) || angle < sf::degrees(-90))
            facingLeft = false;

        performMove(movement, facingLeft);

        if (checkCollision())
        {
            performMove(-movement, facingLeft);

            sf::Vector2f moveX(movement.x, 0.f);
            performMove(moveX, facingLeft);

            if (checkCollision())
            {
                performMove(-moveX, facingLeft);

                sf::Vector2f moveY(0.f, movement.y);
                performMove(moveY, facingLeft);

                if (checkCollision())
                {
                    performMove(-moveY, facingLeft);
                }
            }
        }
    }

    else
    {
        bool facingLeft = true;

        if (angle > sf::degrees(90) || angle < sf::degrees(-90))
            facingLeft = false;
            
        transform({0, 0}, facingLeft, sf::Angle(sf::degrees(0.f)));

        target = position;
    }

    int gridX = static_cast<int>((position.x - GRID_CELL_SIZE) / GRID_CELL_SIZE);
    int gridY = static_cast<int>((position.y - GRID_CELL_SIZE) / GRID_CELL_SIZE);
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

bool Enemy::checkLineOfSight(const sf::Vector2f &origin, const sf::Vector2f &playerPosition, const std::vector<std::unique_ptr<Object>> &obstacles) const
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
        if (obstacle->collidesWith(lineOfSight))
            return false;
    }
    return true;
}

sf::Vector2f Enemy::getPosition() const
{
    return position;
}

std::ostream &operator<<(std::ostream &os, const Enemy &enemy)
{
    os << static_cast<const Entity &>(enemy);
    os << "\n";
    os << "        Enemy Weapon: " << *enemy.weapon;
    return os;
}