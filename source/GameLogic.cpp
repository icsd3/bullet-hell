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
        std::cout << "menu\n";
        break;
    case augment_1:
        augment.load();
        std::cout << "augment_1\n";
        break;
    case augment_2:
        augment.load();
        std::cout << "augment_2\n";
        break;
    case augment_3:
        augment.load();
        std::cout << "augment_3\n";
        break;
    case level_1:
        loader.loadLevel();
        player.load(loader.getPlayerTexture());
        gui.load();
        spawnEnemies(5);
        std::cout << "level_1\n";
        break;
    case level_2:
        loader.loadLevel();
        std::cout << "level_2\n";
        break;
    case level_3:
        loader.loadLevel();
        std::cout << "level_3\n";
        break;
    case defeat:
        // loader.loadDefeat(window);
        std::cout << "defeat\n";
        break;
    case victory:
        // loader.loadVictory(window);
        std::cout << "victory\n";
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
    std::cout << "Paused toggled to " << (paused ? "true" : "false") << "\n";
}

bool Game::handleInputs()
{
    bool shouldExit = false;

    if ((currentState == level_1 || currentState == level_2 || currentState == level_3) && !paused)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        {
            target = mapToLogical(sf::Vector2f(sf::Mouse::getPosition(window)), window);
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            if (player.canFireCurrentWeapon())
            {
                sf::Vector2f projectileTarget = mapToLogical(sf::Vector2f(sf::Mouse::getPosition(window)), window);
                std::vector<Projectile> bullets = player.fireCurrentWeapon(projectileTarget, loader.getPlayerProjectileTexture());
                for(size_t i=0; i<bullets.size(); i++)
                {
                    playerProjectiles.push_back(bullets[i]);
                    playerProjectiles.back().load(loader.getPlayerProjectileTexture());
                }
            }
        }
    }

    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
            std::cout << "Fereastra a fost inchisa fortat\n";
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
            if(openSettings)
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
                    std::cout << "Augment button " << action << " clicked!\n";
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
                    target = mapToLogical(ans.second, window);
                    break;
                case 2:
                    if (player.canFireCurrentWeapon())
                    {
                        sf::Vector2f projectileTarget = mapToLogical(ans.second, window);
                        std::vector<Projectile> bullets = player.fireCurrentWeapon(projectileTarget, loader.getPlayerProjectileTexture());
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
                case 4:
                    shouldExit = true;
                    break;
                default:
                    break;
                }
            }
            else if (event->is<sf::Event::KeyPressed>())
            {
                const auto *keyPressed = event->getIf<sf::Event::KeyPressed>();
                if (keyPressed->scancode == sf::Keyboard::Scancode::Backspace)
                {
                    shouldExit = true;
                    std::cout << "Backspace pressed bye bye!\n";
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::I)
                {
                    std::cout << *this;
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

        if (mouseEvent->button == sf::Mouse::Button::Right)
        {
            return std::make_pair(1, sf::Vector2f(mouseEvent->position));
        }

        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            return std::make_pair(2, sf::Vector2f(mouseEvent->position));
        }
    }
    if (event.is<sf::Event::KeyPressed>())
    {
        const auto *keyPressed = event.getIf<sf::Event::KeyPressed>();
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            return std::make_pair(3, sf::Vector2f(-1.f, -1.f));
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Backspace)
        {
            return std::make_pair(4, sf::Vector2f(-1.f, -1.f));
        }
        // else if(!controls && !paused)
        // {
            
        // }
    }

    return std::make_pair(0, sf::Vector2f(-1.f, -1.f));
}

void Game::spawnEnemies(const int &numberOfEnemies)
{
    for (int i = 0; i < numberOfEnemies; i++)
    {
        enemies.push_back(Enemy::spawnEnemy(loader.getEnemyTexture(), sf::Vector2f(500.f + i * 100.f, 500.f), 100.f, 100, Weapon("EnemyGun", 10, 1, 1.f, 0.f, 0.25f, 0.15f)));
        enemies.back().load(loader.getEnemyTexture());
    }
}

void Game::drawLevel()
{
    loader.drawLevelBackground(window);
    player.draw(window);
    for (auto &projectile : playerProjectiles)
    {
        projectile.draw(window);
    }
    for (auto &enemy : enemies)
    {
        enemy.draw(window);
    }
    drawGUI();
}

void Game::drawGUI()
{
    gui.update(player.getHealthStatus());
    gui.draw(window);
}

// void Game::drawDefeat()
// {

// }

// void Game::drawVictory()
// {

// }

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
        drawLevel();

    case defeat:
        // drawDefeat();
        break;

    case victory:
        // drawVictory();
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

void Game::Play()
{
    while (window.isOpen())
    {
        bool shouldExit = handleInputs();

        if (shouldExit)
        {
            window.close();
            std::cout << "Fereastra a fost inchisa (shouldExit == true)\n";
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
    os << "Loader status:\n"
       << game.loader << "\n\n";
    os << "Player status:\n"
       << game.player << "\n\n";
    os << "GUI status:\n"
       << game.gui << "\n\n";
    os << "Window size: " << game.window.getSize().x << "x" << game.window.getSize().y << "\n\n";
    os << "Current State: " << game.currentState << "\n\n";
    os << "Target Position: (" << game.target.x << ", " << game.target.y << ")\n";
    os << "####################################################################################################################################\n\n";
    return os;
}