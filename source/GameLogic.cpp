#include "../headers/GameLogic.h"
#include "../headers/Utils.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "BulletHell", sf::Style::Default, sf::State::Fullscreen),
      target(sf::Vector2f(LOGICAL_WIDTH / 2.f, LOGICAL_HEIGHT * 0.8f))
{
    setup();
}

Game &Game::getInstance()
{
    static Game instance;
    return instance;
}

void Game::setup()
{
    window.clear(sf::Color::Black);
    window.display();
    loader.loadStaticAssets();
    currentState = main_menu;
    handleNewState();
    window.setVerticalSyncEnabled(true);
    
}

void Game::selectGameState(gameStates &gameState)
{
    switch (gameState)
    {
    case main_menu:
        gameState = level_1;
        // gameState = augment_1;
        break;

    case augment_1:
        gameState = level_1;
        break;

    case level_1:
        gameState = augment_2;
        break;

    case augment_2:
        gameState = level_2;
        break;

    case level_2:
        gameState = augment_3;
        break;

    case augment_3:
        gameState = level_3;
        break;

    case level_3:
        gameState = victory;
        break;

    case defeat:
    case victory:
    default:
        gameState = main_menu;
        break;
    }
}

void Game::handleNewState()
{
    switch (currentState)
    {
    case main_menu:
        menu.load();
        settings.load();
        break;

    case augment_1:
        augment.load();
        break;

    case augment_2:
        augment.load();
        break;

    case augment_3:
        augment.load();
        break;

    case level_1:
        loader.loadLevel();
        player.load(loader.getPlayerTexture());
        gui.load();
        spawnEnemies(4);
        break;

    case level_2:
        loader.loadLevel();
        break;

    case level_3:
        loader.loadLevel();
        break;

    case defeat:
        // loader.loadDefeat(window);
        break;

    case victory:
        // loader.loadVictory(window);
        break;

    default:
        break;
    }
}

void Game::togglePause()
{
    if(!paused)
        updateClock.stop();

    else
        updateClock.start();

    paused = !paused;
}

bool Game::handleInputs()
{
    bool shouldExit = false;
    bool moved = false;

    if ((currentState == level_1 || currentState == level_2 || currentState == level_3) && !paused)
    {
        sf::Vector2f move = handleMovementInput();
        sf::Vector2f shoot = handleShootInput();

        if(move.x > 0 &&  move.x < LOGICAL_WIDTH && move.y > 0 && move.y < LOGICAL_HEIGHT)
        {
            target = move;
            moved = true;
        }

        if((move.x < 0 ||  move.x > LOGICAL_WIDTH) && (move.y > 0 && move.y < LOGICAL_HEIGHT))
        {
            if(move.x < 0)
                move.x = 0;
            else
                move.x = LOGICAL_WIDTH;
            target = move;
            moved = true;
        }

        if((move.y < 0 ||  move.y > LOGICAL_HEIGHT) && (move.x > 0 && move.x < LOGICAL_WIDTH))
        {
            if(move.y < 0)
                move.y = 0;
            else
                move.y = LOGICAL_HEIGHT;
            target = move;
            moved = true;
        }

        if (shoot.x != -1 && shoot.y != -1 && player.canFire())
        {
            sf::Vector2f projectileTarget = shoot;
            std::vector<Projectile> bullets = player.fire(projectileTarget, loader.getPlayerProjectileTexture());
            
            for(size_t i=0; i<bullets.size(); i++)
            {
                playerProjectiles.push_back(bullets[i]);
                playerProjectiles.back().load(loader.getPlayerProjectileTexture());
            }
        }
    }

    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        else if(event->is<sf::Event::FocusLost>())
        {
            if (currentState == level_1 || currentState == level_2 || currentState == level_3)
            {
                if(!paused)
                {
                    togglePause();
                    openSettings = true;
                }
            }
        }

        else if (event->is<sf::Event::Resized>())
        {
            const auto* resized = event->getIf<sf::Event::Resized>();
            sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(resized->size));
            window.setView(sf::View(visibleArea));
        }

        else if (event->is<sf::Event::KeyPressed>() || event->is<sf::Event::MouseButtonPressed>())
        {
            if(event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Backspace)
                shouldExit = true;

            else if(event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::I)
                std::cout << *this;

            else if(openSettings)
            {
                int action = settings.handleInput(window, *event);

                switch (action)
                {
                case 1:
                    window.close();

                    if(fullscreen)
                        window.create(sf::VideoMode::getDesktopMode(), "BulletHell", sf::Style::Default, sf::State::Windowed);
                    
                    else
                        window.create(sf::VideoMode::getDesktopMode(), "BulletHell", sf::Style::Default, sf::State::Fullscreen);
                    
                    fullscreen = !fullscreen;
                    window.setVerticalSyncEnabled(true);
                    window.clear(sf::Color::Black);
                    window.display();
                    break;

                case 2:
                    controls = !controls;
                    break;

                case 3:
                    openSettings = false;

                    if(paused)
                        togglePause();

                    break;

                default:
                    break;
                }
            }

            else if(currentState == main_menu)
            {
                int action = menu.handleInput(window, *event);

                switch (action)
                {
                case 1:
                    selectGameState(currentState);
                    handleNewState();
                    break;

                case 2:
                    openSettings = true;
                    break;

                case 3:
                    shouldExit = true;
                    break;

                default:
                    break;
                }
            }

            else if (currentState == augment_1 || currentState == augment_2 || currentState == augment_3)
            {
                int action = augment.handleInput(window, *event);

                if (action != 0)
                {
                    selectGameState(currentState);
                    handleNewState();
                }
            }

            else if (currentState == level_1 || currentState == level_2 || currentState == level_3)
            {
                std::pair<int, sf::Vector2f> ans = handleLevelInput(*event);
                int action = ans.first;

                switch (action)
                {
                case 1:
                    if(!moved && ans.second.x > 0 && ans.second.y > 0 && ans.second.x < LOGICAL_WIDTH && ans.second.y < LOGICAL_HEIGHT)
                        target = ans.second;
                    break;

                case 2:
                    if (player.canFire())
                    {
                        sf::Vector2f projectileTarget = ans.second;
                        std::vector<Projectile> bullets = player.fire(projectileTarget, loader.getPlayerProjectileTexture());
                        
                        for(size_t i=0; i<bullets.size(); i++)
                        {
                            playerProjectiles.push_back(bullets[i]);
                            playerProjectiles.back().load(loader.getPlayerProjectileTexture());
                        }
                    }
                    break;

                case 3:
                    togglePause();
                    openSettings = !openSettings;   
                    break;

                default:
                    break;
                }
            }
        }
    }
    return shouldExit;
}

std::pair<int, sf::Vector2f> Game::handleLevelInput(const sf::Event &event)
{
    if (event.is<sf::Event::MouseButtonPressed>() && !paused)
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

        else if(controls && !paused)
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

sf::Vector2f Game::handleMovementInput()
{
    if(!paused)
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
    }
    return sf::Vector2f(-1,-1);
}

sf::Vector2f Game::handleShootInput()
{
    if(!paused)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            return mapToLogical(sf::Vector2f(sf::Mouse::getPosition(window)), window);
        }
    }
    return sf::Vector2f(-1.f, -1.f);
}

void Game::spawnEnemies(const int &numberOfEnemies)
{
    for (int i = 1; i <= numberOfEnemies; i++)
    {
        enemies.push_back(Enemy::spawnEnemy(loader.getEnemyTexture(), sf::Vector2f(100.f + i%2 * 1720.f, 100.f + (i-1) / 2 * 880.f), 100.f, 100));
        enemies.back().load(loader.getEnemyTexture());
    }
}

void Game::draw()
{
    window.clear();

    switch (currentState)
    {
    case main_menu:
        menu.draw(window);
        break;

    case augment_1:
    case augment_2:
    case augment_3:
        augment.draw(window);
        break;

    case level_1:
    case level_2:
    case level_3:
        loader.drawLevelBackground(window);
        player.draw(window);

        for (auto &projectile : playerProjectiles)
            projectile.draw(window);

        for (auto &projectile : enemyProjectiles)
            projectile.draw(window);

        for (auto &enemy : enemies)
            enemy.draw(window);

        gui.update(player.getHealthStatus());
        gui.draw(window);
        break;

    case defeat:
        
        break;

    case victory:
        
        break;

    default:
        break;
    }

    if(openSettings)
    {
        settings.draw(window);
    }

    window.display();
}

void Game::updateEntities()
{
    float dt = updateClock.restart().asSeconds();
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

    for(size_t i = 0; i < enemies.size(); i++)
    {
        enemies[i].update();
        if(enemies[i].canFire())
        {
            sf::Vector2f projectileTarget = player.getPosition();
            std::vector<Projectile> bullets = enemies[i].fire(projectileTarget, loader.getEnemyProjectileTexture());
            
            for(size_t j=0; j<bullets.size(); j++)
            {
                enemyProjectiles.push_back(bullets[j]);
                enemyProjectiles.back().load(loader.getEnemyProjectileTexture());
            }
        }
    }

    for (size_t i = 0; i < enemyProjectiles.size();)
    {
        if (enemyProjectiles[i].update(dt))
            enemyProjectiles.erase(enemyProjectiles.begin() + i);

        else
        {
            if (checkPlayerHits(enemyProjectiles[i]))
                enemyProjectiles.erase(enemyProjectiles.begin() + i);

            else
                i++;
        }
    }
}

bool Game::checkEnemyHits(const Projectile &projectile)
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

bool Game::checkPlayerHits(const Projectile &projectile)
{
    if (projectile.collidesWith(player))
    {
        if (player.takeDamage(projectile.getDamage()))
            window.close();
        return true;
    }  
    return false;
}

void Game::Play()
{
    while (window.isOpen())
    {
        bool shouldExit = handleInputs();

        if (shouldExit)
        {
            window.close();
            break;
        }

        if ((currentState == level_1 || currentState == level_2 || currentState == level_3) && !paused)
        {
            updateEntities();
        }

        draw();
    }
}

std::ostream &operator<<(std::ostream &os, const Game &game)
{
    os << "\n####################################################################################################################################\n";
    os << game.loader << "\n";
    os << game.menu << "\n";
    os << game.augment << "\n";
    os << game.settings << "\n";
    os << game.player << "\n";
    os << "Enemies:\n";
    os << "    Count: " << game.enemies.size() << "\n";
    if(game.enemies.size() != 0)
        for(size_t i=0; i<game.enemies.size(); i++)
        {
            os << "    Enemy " << i+1 << ":\n        "
            << game.enemies[i] << "\n\n";
        }
    else 
        os << "\n";
    os << "Player projectiles:\n"
       << "    Count: " << game.playerProjectiles.size() << "\n";
    if(game.playerProjectiles.size() != 0)
        for(size_t i=0; i<game.playerProjectiles.size(); i++)
        {
            os << "    Projectile " << i+1 << ":\n        "
            << game.playerProjectiles[i] << "\n\n";
        }
    else
        os << "\n";
    os << "Enemy projectiles:\n"
       << "    Count: " << game.enemyProjectiles.size() << "\n";
    if(game.enemyProjectiles.size() != 0)
        for(size_t i=0; i<game.enemyProjectiles.size(); i++)
        {
            os << "    Projectile " << i+1 << ":\n        "
           << game.enemyProjectiles[i] << "\n\n";
        }
    else 
        os << "\n";
    os << "GUI status:\n"
       << game.gui << "\n\n";
    os << "Window size: " << game.window.getSize().x << "x" << game.window.getSize().y << "\n\n";
    os << "Current State: " << game.currentState << "\n\n";
    os << "Target Position: (" << game.target.x << ", " << game.target.y << ")\n";
    os << "####################################################################################################################################\n\n";
    return os;
}