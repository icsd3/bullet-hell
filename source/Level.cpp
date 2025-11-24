#include "../headers/Level.h"
#include "../headers/Utils.h"

Level::Level()
    :playerTexturePath("textures/player.png"),
    enemyPath("textures/enemy.png"),
    playerProjectilePath("textures/player_projectile.png"),
    enemyProjectilePath("textures/enemy_projectile.png"),
    playerTexture(playerTexturePath),
    enemyTexture(enemyPath),
    playerProjectileTexture(playerProjectilePath),
    enemyProjectileTexture(enemyProjectilePath)
{
}

Level &Level::getInstance()
{
    static Level instance;
    return instance;
}

void Level::load()
{
    currentRoom.load();
    player.load({LOGICAL_WIDTH / 2, LOGICAL_HEIGHT / 2});
}

void Level::spawnEnemies(const int &nrOfEnemies)
{
    for (int i = 1; i <= nrOfEnemies; i++)
    {
        enemies.push_back(Enemy::spawnEnemy(enemyTexture, sf::Vector2f(100.f + i%2 * 1720.f, 100.f + (i-1) / 2 * 880.f), 100.f, 100));
        enemies.back().load(enemyTexture);
    }
}

std::pair<int, sf::Vector2f> Level::handleInput(const sf::Event &event, const bool& controls, const sf::RenderWindow &window)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouseEvent->button == sf::Mouse::Button::Right && !controls)
        {
            return std::make_pair(1, mapToLogical(sf::Vector2f(mouseEvent->position), window));
        }

        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            return std::make_pair(2, mapToLogical(sf::Vector2f(mouseEvent->position), window));
        }
    }
    if (event.is<sf::Event::KeyPressed>())
    {
        const auto *keyPressed = event.getIf<sf::Event::KeyPressed>();

        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            return std::make_pair(3, sf::Vector2f(-1.f, -1.f));
        }

        else if(controls)
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::W)
                return std::make_pair(1, player.getPosition() + sf::Vector2f(0.f, -10.f));

            else if (keyPressed->scancode == sf::Keyboard::Scancode::A)
                return std::make_pair(1, player.getPosition() + sf::Vector2f(-10.f, 0.f));

            else if (keyPressed->scancode == sf::Keyboard::Scancode::S)
                return std::make_pair(1, player.getPosition() + sf::Vector2f(0.f, 10.f));

            else if (keyPressed->scancode == sf::Keyboard::Scancode::D)
                return std::make_pair(1, player.getPosition() + sf::Vector2f(10.f, 0.f));
        }
    }

    return std::make_pair(0, sf::Vector2f(-1.f, -1.f));
}

sf::Vector2f Level::handleMovementInput(const bool &controls, const sf::RenderWindow &window)
{
    if(controls)
    {
        sf::Vector2f movement = player.getPosition();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            movement += sf::Vector2f(0.f, -10.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            movement += sf::Vector2f(-10.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            movement += sf::Vector2f(0.f, 10.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            movement += sf::Vector2f(10.f, 0.f);

        return movement; 
    } 
    else
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        {
            return mapToLogical(sf::Vector2f(sf::Mouse::getPosition(window)), window);
        }
    }
    return {-1.f, -1.f};
}

sf::Vector2f Level::handleShootInput(const sf::RenderWindow &window)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        return mapToLogical(sf::Vector2f(sf::Mouse::getPosition(window)), window);
    }
    return {-1.f, -1.f};
}

void Level::draw(sf::RenderWindow &window)
{
    currentRoom.draw(window);

    for (auto &projectile : playerProjectiles)
        projectile.draw(window);

    for (auto &projectile : enemyProjectiles)
        projectile.draw(window);

    for (auto &enemy : enemies)
        enemy.draw(window);

    player.draw(window);
}

void Level::updateEntities(const float &dt, const sf::Vector2f &target)
{
    player.update(dt, target);
    for (size_t i = 0; i < playerProjectiles.size();)
    {
        if (playerProjectiles[i].update(dt))
            playerProjectiles.erase(playerProjectiles.begin() + i);

        else
        {
            if (checkEnemyHits(playerProjectiles[i]))
                playerProjectiles.erase(playerProjectiles.begin() + i);

            else
                i++;
        }
    }

    for(auto &enemy : enemies)
    {
        enemy.update();
        if(enemy.canFire())
        {
            sf::Vector2f projectileTarget = player.getPosition();
            std::vector<Projectile> bullets = enemy.fire(projectileTarget, enemyProjectileTexture);
            
            for(auto &bullet : bullets)
            {
                enemyProjectiles.push_back(bullet);
                enemyProjectiles.back().load(enemyProjectileTexture);
            }
        }
    }

    for (size_t i = 0; i < enemyProjectiles.size();)
    {
        if (enemyProjectiles[i].update(dt))
            enemyProjectiles.erase(enemyProjectiles.begin() + i);

        else
        {
            if (checkPlayerHits(enemyProjectiles[i], player))
                enemyProjectiles.erase(enemyProjectiles.begin() + i);

            else
                i++;
        }
    }
}

bool Level::checkEnemyHits(const Projectile &projectile)
{
    for (size_t i = 0; i < enemies.size();)
    {
        if (projectile.collidesWith(enemies[i]))
        {
            if (enemies[i].takeDamage(projectile.getDamage()))
                enemies.erase(enemies.begin() + i);
            return true;
        }

        else
            i++;
    }
    return false;
}

bool Level::checkPlayerHits(const Projectile &projectile, Player &player)
{
    if (projectile.collidesWith(player))
    {
        if (player.takeDamage(projectile.getDamage()))
            std::cout<<"game over\n";
        return true;
    }  
    return false;
}

void Level::spawnPlayerProjectile(const sf::Vector2f &target)
{
    std::vector<Projectile> bullets = player.fire(target, playerProjectileTexture);
            
    for(auto &bullet : bullets)
    {
        playerProjectiles.push_back(bullet);
        playerProjectiles.back().load(playerProjectileTexture);
    }
}