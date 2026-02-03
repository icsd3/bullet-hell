#include "../headers/BossEnemy.h"

BossEnemy::BossEnemy(const sf::Vector2f &pos, float spd, const int &mh)
    : Enemy(pos, spd, mh, ResourceManager::getTexture(TextureType::Boss))
{
    weapon = std::make_unique<Gun>("json/EnemyGuns.json", 3, 0.0f, AttackTextureType::Boss);
    cannon = std::make_unique<Gun>("json/EnemyGuns.json", 4, 0.0f, AttackTextureType::Boss);
    laser = std::make_unique<Laser>("json/EnemyLasers.json", 1, 0.0f, AttackTextureType::Boss);
}

void BossEnemy::doLoad()
{
    sf::Vector2f scaleFactor = {120.f, 0.f};
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
    cannon->load(position);
    cannon->reset();
    laser->load(position);
    laser->reset();
    updateClock.restart();

    offset = sf::degrees(0.f);
}

std::vector<std::unique_ptr<Attack>> BossEnemy::doUpdate(const float &dt, const sf::Vector2f &playerPosition, const RoomElements &elements, const std::vector<std::unique_ptr<Enemy>> &enemies, int grid[GRID_SIZE_X][GRID_SIZE_Y])
{
    sf::Angle angle = (playerPosition - position).angle();

    weapon->update(position, angle);
    cannon->update(position, angle);
    laser->update(position, angle);

    std::vector<std::unique_ptr<Attack>> attacks;
    std::vector<std::unique_ptr<Attack>> newAttacks;

    newAttacks = weapon->attack(position, playerPosition);
    std::move(newAttacks.begin(), newAttacks.end(), std::back_inserter(attacks));

    newAttacks = cannon->attack(position, playerPosition);
    std::move(newAttacks.begin(), newAttacks.end(), std::back_inserter(attacks));

    sf::Vector2f offsetVector = {100.f, 0.f};
    newAttacks = laser->attack(position, position + offsetVector.rotatedBy(offset));
    std::move(newAttacks.begin(), newAttacks.end(), std::back_inserter(attacks));

    if (newAttacks.size() > 0)
    {
        offset += sf::degrees(30.f);
        offset = offset % sf::degrees(360.f);
    }

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