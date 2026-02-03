#include "../headers/GunEnemy.h"

GunEnemy::GunEnemy(const sf::Vector2f &pos, float spd, const int &mh)
    : Enemy(pos, spd, mh, ResourceManager::getTexture(TextureType::Enemy))
{
    std::mt19937 &rng = Utils::getRng();
    std::uniform_int_distribution<int> enemyGunIndexRange(0, 2);
    int enemyGunIndex = enemyGunIndexRange(rng);
    float fireRateMaxOffset = 0.3f;

    weapon = std::make_unique<Gun>("json/EnemyGuns.json", enemyGunIndex, fireRateMaxOffset, AttackTextureType::Enemy);
}

std::vector<std::unique_ptr<Attack>> GunEnemy::doUpdate(const float &dt, const sf::Vector2f &playerPosition, const RoomElements &elements, const std::vector<std::unique_ptr<Enemy>> &enemies, int grid[GRID_SIZE_X][GRID_SIZE_Y])
{
    sf::Vector2i playerGridPosition(static_cast<int>((playerPosition.x - GRID_CELL_SIZE) / GRID_CELL_SIZE), static_cast<int>((playerPosition.y - GRID_CELL_SIZE) / GRID_CELL_SIZE));

    sf::Angle angle = (playerPosition - position).angle();

    weapon->update(position, angle);

    std::vector<std::unique_ptr<Attack>> attacks;

    if (checkLineOfSight(position, playerPosition, elements.obstacles))
    {
        attacks = weapon->attack(position, playerPosition);
        
        sf::Vector2f dir = target - position;
        float distToTarget = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (distToTarget < 30.0f || updateClock.getElapsedTime().asSeconds() > 2.0f)
        {
            updateClock.restart();
            std::vector<sf::Vector2i> validCells;
            const sf::Vector2i directions[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

            for (const auto &dir : directions)
            {
                int nx = gridPosition.x + dir.x;
                int ny = gridPosition.y + dir.y;

                if (nx >= 0 && nx < GRID_SIZE_X && ny >= 0 && ny < GRID_SIZE_Y && grid[nx][ny] != 1)
                {
                    validCells.push_back({nx, ny});
                }
            }

            if (!validCells.empty())
            {
                std::mt19937 &rng = Utils::getRng();
                std::uniform_int_distribution<size_t> dist(0, validCells.size() - 1);
                sf::Vector2i newTarget = validCells[dist(rng)];
                target = sf::Vector2f(BORDER_SIZE + newTarget.x * GRID_CELL_SIZE, BORDER_SIZE + newTarget.y * GRID_CELL_SIZE);
            }
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

sf::Vector2f GunEnemy::nextPathPoint(const sf::Vector2i &start, const sf::Vector2i &goal, const int grid[GRID_SIZE_X][GRID_SIZE_Y])
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

bool GunEnemy::checkLineOfSight(const sf::Vector2f &origin, const sf::Vector2f &playerPosition, const std::vector<std::unique_ptr<Object>> &obstacles) const
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