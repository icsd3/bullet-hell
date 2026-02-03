#include "../headers/Enemy.h"

Enemy::Enemy(const sf::Vector2f &pos, float spd, const int &mh, const sf::Texture &tex)
    : Entity(pos, tex, spd, mh), gridPosition(0, 0), target(pos)
{
}

void Enemy::doLoad()
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

void Enemy::load()
{
    doLoad();
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
    return doUpdate(dt, playerPosition, elements, enemies, grid);
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