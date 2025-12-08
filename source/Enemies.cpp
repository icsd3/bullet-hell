#include "../headers/Enemies.h"
#include "../headers/Utils.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <cfloat>
#include <cmath>

Enemy::Enemy(const sf::Vector2f &pos, const bool &ori, sf::Texture &tex, const float spd, const int &mh, const Weapon &ew)
    : Entity(pos, ori, tex, spd, mh), weapon(ew), gridPosition(0, 0), target(pos)
{
    int gridX = static_cast<int>((pos.x - 120.f) / 120.f);
    int gridY = static_cast<int>((pos.y - 120.f) / 120.f);
    gridPosition = {gridX, gridY};
}

Enemy Enemy::spawnEnemy(sf::Texture &tex, const sf::Vector2f &pos, float spd, const int &mh, sf::Texture &prtex)
{
    std::ifstream file("json/Weapons.json");
    nlohmann::json data;
    file >> data;
    const auto &w = data[0];
    Weapon ew = Weapon(
        w["name"],
        w["damage"],
        w["bullet_nr"],
        w["fire_rate"],
        w["spread_angle"],
        w["range"],
        w["bullet_speed"],
        0.3f,
        prtex
    );
    Enemy enemy(pos, false, tex, spd, mh, ew);
    return enemy;
}

void Enemy::doLoad()
{
    Entity::doLoad();

    maxHealthBar.setSize(sf::Vector2f(sprite.value().getGlobalBounds().size.x, LOGICAL_HEIGHT / 100.f));
    maxHealthBar.setFillColor(sf::Color(75, 0, 0, 175));
    maxHealthBar.setOrigin(sf::Vector2f(maxHealthBar.getLocalBounds().size.x / 2.f, maxHealthBar.getLocalBounds().size.y / 2.f));
    maxHealthBar.setPosition(sf::Vector2f(position.x, position.y - sprite.value().getGlobalBounds().size.y / 2.f - maxHealthBar.getSize().y));
    maxHealthBar.setOutlineThickness(2.f);
    maxHealthBar.setOutlineColor(sf::Color::Black);

    currentHealthBar.setSize(maxHealthBar.getSize());
    currentHealthBar.setFillColor(sf::Color(150, 0, 0, 175));
    currentHealthBar.setOrigin(sf::Vector2f(0, currentHealthBar.getLocalBounds().size.y / 2.f));
    currentHealthBar.setPosition(sf::Vector2f(maxHealthBar.getPosition().x - maxHealthBar.getSize().x / 2.f, maxHealthBar.getPosition().y));

    weapon.reset();
}

void Enemy::doDraw(sf::RenderWindow &window)
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

std::vector<Projectile> Enemy::update(const float &dt, const sf::Vector2f &playerPosition, const std::vector<Object> &obstacles, const std::vector<Object> &walls, const std::vector<Door> &doors, const std::vector<Enemy> &enemies, int grid[14][7])
{
    sf::Vector2i playerGridPosition(static_cast<int>((playerPosition.x - 120.f) / 120.f), static_cast<int>((playerPosition.y - 120.f) / 120.f));

    weapon.update();

    std::vector<Projectile> bullets;

    if (doCheckLineOfSight(position, playerPosition, obstacles))
    {
        bullets = weapon.fire(position, playerPosition);
        target = position;
    }

    else if (updateClock.getElapsedTime().asSeconds() >= 0.1f)
    {
        target = nextPathPoint(gridPosition, playerGridPosition, grid);
        updateClock.restart();
    }

    move(dt, obstacles, walls, doors, enemies);

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

sf::Vector2f Enemy::nextPathPoint(const sf::Vector2i& start, const sf::Vector2i& goal, const int grid[14][7])
{
    struct Node {
        int x, y;
        float g, h;
        Node* parent;
        float f() const { return g + h; }
    };

    auto inBounds = [&](int x, int y) { return x >= 0 && x < 14 && y >= 0 && y < 7; };

    auto heuristic = [](int x1, int y1, int x2, int y2) {
        float dx = static_cast<float>(x2 - x1);
        float dy = static_cast<float>(y2 - y1);
        return std::sqrt(dx * dx + dy * dy);
    };

    const std::pair<sf::Vector2i, float> directions[8] = {
        {{1, 0}, 1.f}, {{-1, 0}, 1.f}, {{0, 1}, 1.f}, {{0, -1}, 1.f},
        {{1, 1}, 1.4142f}, {{1, -1}, 1.4142f}, {{-1, 1}, 1.4142f}, {{-1, -1}, 1.4142f}
    };

    bool closed[7][14] = {{false}};
    std::unique_ptr<Node> nodes[7][14];

    auto cmp = [](const Node* a, const Node* b) 
    {
        return a->f() > b->f();
    };
    
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> open(cmp);

    nodes[start.y][start.x] = std::make_unique<Node>(Node{start.x, start.y, 0.f, heuristic(start.x, start.y, goal.x, goal.y), nullptr});
    open.push(nodes[start.y][start.x].get());

    while (!open.empty()) {
        Node* current = open.top();
        open.pop();

        if (current->x == goal.x && current->y == goal.y) {
            Node* step = current;
            while (step->parent && (step->parent->x != start.x || step->parent->y != start.y))
                step = step->parent;
            return sf::Vector2f(180.f + step->x * 120.f, 180.f + step->y * 120.f);
        }

        closed[current->y][current->x] = true;

        for (auto& [dir, cost] : directions) {
            int nx = current->x + dir.x;
            int ny = current->y + dir.y;

            if (!inBounds(nx, ny) || grid[nx][ny] == 1 || closed[ny][nx])
                continue;

            if (dir.x != 0 && dir.y != 0) 
                if (grid[current->x][current->y + dir.y] == 1 || grid[current->x + dir.x][current->y] == 1)
                    continue;

            float penalty = (grid[nx][ny] >= 2) ? 5.0f : 1.0f;
            float newG = current->g + (cost * penalty);

            if (!nodes[ny][nx]) {
                nodes[ny][nx] = std::make_unique<Node>(Node{nx, ny, newG, heuristic(nx, ny, goal.x, goal.y), current});
                open.push(nodes[ny][nx].get());
            }
            else if (newG < nodes[ny][nx]->g) {
                nodes[ny][nx]->g = newG;
                nodes[ny][nx]->parent = current;
            }
        }
    }
    
    return sf::Vector2f(180.f + start.x * 120.f, 180.f + start.y * 120.f); 
}

void Enemy::move(const float &dt, const std::vector<Object> &obstacles, const std::vector<Object> &walls, const std::vector<Door> &doors, const std::vector<Enemy> &enemies)
{
    sf::Vector2f separation(0.f, 0.f);
    int neighbors = 0;

    for (const auto &enemy : enemies)
    {
        if (&enemy == this) 
            continue;

        sf::Vector2f diff = position - enemy.position;
        float distSq = diff.x * diff.x + diff.y * diff.y;
    
        if (distSq < 8100.f && distSq > 0.1f)
        {
            separation += diff / std::sqrt(distSq);
            neighbors++;
        }
    }

    sf::Vector2f pathDir(0.f, 0.f);
    sf::Vector2f diff = target - position;
    float distToTarget = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (distToTarget > 5.0f)
    {
        pathDir = diff / distToTarget;
    }

    sf::Vector2f finalDir = pathDir;

    if (neighbors > 0)
    {
        finalDir += separation * 1.5f;
    }

    float finalLen = std::sqrt(finalDir.x * finalDir.x + finalDir.y * finalDir.y);
    if (finalLen > 0.5f)
    {
        finalDir /= finalLen;

        sf::Vector2f movement = finalDir * speed * LOGICAL_WIDTH * dt;

        auto checkCollision = [&]() -> bool {
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
                if (&enemy != this && collidesWith(enemy)) 
                    return true;
            return false;
        };
        
        auto performMove = [&](const sf::Vector2f& moveVec) {
            sprite.value().move(moveVec);
            collisionBox.move(moveVec);
            hitBox.move(moveVec);
            maxHealthBar.move(moveVec);
            currentHealthBar.move(moveVec);
            position = sprite.value().getPosition();
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
        
        if (movement.x > 0.f)
        {
            orientation = true;
            sprite.value().setScale(sf::Vector2f(-std::abs(sprite.value().getScale().x), sprite.value().getScale().y));
        }
        else
        {
            orientation = false;
            sprite.value().setScale(sf::Vector2f(std::abs(sprite.value().getScale().x), sprite.value().getScale().y));
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
        return true;
    return false;
}

bool Enemy::doCheckLineOfSight(const sf::Vector2f &origin, const sf::Vector2f &playerPosition, const std::vector<Object> &obstacles) const
{
    for (const auto &obstacle : obstacles)
    {
        if (obstacle.lineIntersects(origin, playerPosition))
            return false;
    }
    return true;
}

std::ostream &operator<<(std::ostream &os, const Enemy &enemy)
{
    os << static_cast<const Entity &>(enemy);
    os << "\n";
    os << "        Max Health: " << enemy.maxHealth << "\n";
    os << "        Current Health: " << enemy.currentHealth << "\n";
    os << "        Enemy Weapon: " << enemy.weapon;
    return os;
}
