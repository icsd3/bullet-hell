#include "../headers/LaserEnemy.h"

LaserEnemy::LaserEnemy(const sf::Vector2f &pos, float spd, const int &mh)
    : Enemy(pos, spd, mh, ResourceManager::getTexture(TextureType::Enemy))
{
    weapon = std::make_unique<Laser>("json/EnemyLasers.json", 0, 0.0f, AttackTextureType::Enemy);
}

std::vector<std::unique_ptr<Attack>> LaserEnemy::doUpdate(const float &dt, const sf::Vector2f &playerPosition, const RoomElements &elements, const std::vector<std::unique_ptr<Enemy>> &enemies, int grid[GRID_SIZE_X][GRID_SIZE_Y])
{
    sf::Angle angle = (playerPosition - position).angle();

    weapon->update(position, angle);

    std::vector<std::unique_ptr<Attack>> attacks;

    attacks = weapon->attack(position, playerPosition);

    sf::Vector2f dir = target - position;
    float distToTarget = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (distToTarget < 30.0f || updateClock.getElapsedTime().asSeconds() > 5.0f)
    {
        updateClock.restart();
        std::vector<sf::Vector2i> validCells;
        const sf::Vector2i directions[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        for (const auto &dire : directions)
        {
            int nx = gridPosition.x + dire.x;
            int ny = gridPosition.y + dire.y;

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

    enemyMove(dt, elements, enemies, angle);

    int gridX = static_cast<int>((position.x - GRID_CELL_SIZE) / GRID_CELL_SIZE);
    int gridY = static_cast<int>((position.y - GRID_CELL_SIZE) / GRID_CELL_SIZE);
    gridPosition = {gridX, gridY};

    return attacks;
}