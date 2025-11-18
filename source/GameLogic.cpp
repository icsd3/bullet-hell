#include "../headers/GameLogic.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "BulletHell", sf::Style::Default, sf::State::Fullscreen),
      target(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y * 0.8f))
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
        gameState = augment_1;
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
        menu.load(window);
        settings.loadSettingsBox(window, fullscreen, controls);
        std::cout << "menu\n";
        break;
    case augment_1:
        loader.loadAugment(window);
        std::cout << "augment_1\n";
        break;
    case augment_2:
        loader.loadAugment(window);
        std::cout << "augment_2\n";
        break;
    case augment_3:
        loader.loadAugment(window);
        std::cout << "augment_3\n";
        break;
    case level_1:
        loader.loadLevel(window);
        player.loadPlayer(window, loader.getPlayerTexture());
        gui.loadGUI(window);
        spawnEnemies(5);
        std::cout << "level_1\n";
        break;
    case level_2:
        loader.loadLevel(window);
        std::cout << "level_2\n";
        break;
    case level_3:
        loader.loadLevel(window);
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
            target = sf::Vector2f(sf::Mouse::getPosition());
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            if (player.canFireCurrentWeapon())
            {
                sf::Vector2f projectileTarget = sf::Vector2f(sf::Mouse::getPosition());
                playerProjectiles.push_back(player.fireCurrentWeapon(projectileTarget, loader.getPlayerProjectileTexture()));
                playerProjectiles.back().loadProjectile(window, loader.getPlayerProjectileTexture());
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
                    togglePause();
            }
        }
        else if (event->is<sf::Event::KeyPressed>() || event->is<sf::Event::MouseButtonPressed>())
        {
            if(currentState == main_menu)
            {
                int action = menu.handleInput(*event, window);
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
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                {
                    if (currentState == level_1 || currentState == level_2 || currentState == level_3)
                    {
                        togglePause();
                    }
                }
            }
            else if (currentState == augment_1 || currentState == augment_2 || currentState == augment_3)
                handleAugmentInput(*event);
            else if (currentState == level_1 || currentState == level_2 || currentState == level_3)
                handleLevelInput(*event);
        }
    }
    return shouldExit;
}

void Game::handleAugmentInput(const sf::Event &event)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            if (!loader.hasAugmentButtonSprites())
                return;

            for (int i = 0; i < 3; i++)
            {
                const auto buttonBounds = loader.getAugmentButtonBounds(i);

                if (buttonBounds.contains(sf::Vector2f(mouseEvent->position)))
                {
                    std::cout << "Augment button " << i + 1 << " clicked!\n";
                    selectGameState(currentState);
                    handleNewState();
                    break;
                }
            }
        }
    }
}

void Game::handleLevelInput(const sf::Event &event)
{
    if (event.is<sf::Event::MouseButtonPressed>() && !paused)
    {
        const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouseEvent->button == sf::Mouse::Button::Right)
        {
            target = sf::Vector2f(mouseEvent->position);
        }

        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            if (player.canFireCurrentWeapon())
            {
                sf::Vector2f projectileTarget = sf::Vector2f(mouseEvent->position);
                playerProjectiles.push_back(player.fireCurrentWeapon(projectileTarget, loader.getPlayerProjectileTexture()));
                playerProjectiles.back().loadProjectile(window, loader.getPlayerProjectileTexture());
                // target = player.getPosition();
            }
        }
    }
    if (event.is<sf::Event::KeyPressed>())
    {
        // const auto* keyBoardEvent = event.getIf<sf::Event::KeyPressed>();
    }
}

void Game::spawnEnemies(const int &numberOfEnemies)
{
    for (int i = 0; i < numberOfEnemies; i++)
    {
        enemies.push_back(Enemy::spawnEnemy(loader.getEnemyTexture(), sf::Vector2f(500.f + i * 100.f, 500.f), 100.f, 100, Weapon("EnemyGun", 10, 1, 1.f, 0.f, 500.f, 300.f)));
        enemies.back().loadEnemy(window, loader.getEnemyTexture());
    }
}

void Game::drawLevel()
{
    loader.drawLevelBackground(window);
    player.drawPlayer(window);
    for (auto &projectile : playerProjectiles)
    {
        projectile.drawProjectile(window);
    }
    for (auto &enemy : enemies)
    {
        enemy.drawEnemy(window);
    }
    drawGUI();
    if(paused)
    {
        settings.draw(window, fullscreen, controls);
    }
}

void Game::drawGUI()
{
    gui.updateGUI(player.getHealthStatus(), window);
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
        loader.drawAugment(window);
        break;

    case level_1:
    case level_2:
    case level_3:
        drawLevel();
        break;

    case defeat:
        // drawDefeat();
        break;

    case victory:
        // drawVictory();
        break;

    default:
        break;
    }

    window.display();
}

void Game::updateEntities()
{
    float dt = updateClock.restart().asSeconds();
    player.updatePlayer(dt, target);
    for (size_t i = 0; i < playerProjectiles.size();)
    {
        if (playerProjectiles[i].updateProjectile(dt, window))
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