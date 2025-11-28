#include "../headers/GameLogic.h"
#include "../headers/Utils.h"
#include <iostream>

Game::Game()
    : target(sf::Vector2f(LOGICAL_WIDTH / 2.f, LOGICAL_HEIGHT * 0.8f)),
      window(sf::VideoMode::getDesktopMode(), "BulletHell", sf::Style::Default, sf::State::Fullscreen)
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
        updateClock.restart();
        level.load(1);
        break;

    case level_2:
        
        break;

    case level_3:
        
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
        sf::Vector2f move = level.handleMovementInput(controls, window);
        sf::Vector2f shoot = level.handleShootInput(window);

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
            level.spawnPlayerProjectile(shoot);
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

            else if ((currentState == level_1 || currentState == level_2 || currentState == level_3) && !paused)
            {
                std::pair<int, sf::Vector2f> ans = level.handleInput(*event, controls, window);
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
                        level.spawnPlayerProjectile(ans.second);
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
        level.draw(window);
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
            float dt = updateClock.restart().asSeconds();
            level.update(dt, target);
        }
        draw();
    }
}

std::ostream &operator<<(std::ostream &os, const Game &game)
{
    os << "\n####################################################################################################################################\n";
    os << game.menu << "\n";
    os << game.augment << "\n";
    os << game.settings << "\n";
    os << game.player << "\n";
    // os << "Enemies:\n";
    // os << "    Count: " << game.enemies.size() << "\n";
    // if(game.enemies.size() != 0)
    //     for(size_t i=0; i<game.enemies.size(); i++)
    //     {
    //         os << "    Enemy " << i+1 << ":\n        "
    //         << game.enemies[i] << "\n\n";
    //     }
    // else 
    //     os << "\n";
    // os << "Player projectiles:\n"
    //    << "    Count: " << game.playerProjectiles.size() << "\n";
    // if(game.playerProjectiles.size() != 0)
    //     for(size_t i=0; i<game.playerProjectiles.size(); i++)
    //     {
    //         os << "    Projectile " << i+1 << ":\n        "
    //         << game.playerProjectiles[i] << "\n\n";
    //     }
    // else
    //     os << "\n";
    // os << "Enemy projectiles:\n"
    //    << "    Count: " << game.enemyProjectiles.size() << "\n";
    // if(game.enemyProjectiles.size() != 0)
    //     for(size_t i=0; i<game.enemyProjectiles.size(); i++)
    //     {
    //         os << "    Projectile " << i+1 << ":\n        "
    //        << game.enemyProjectiles[i] << "\n\n";
    //     }
    // else 
    //     os << "\n";
    // os << "GUI status:\n"
    //    << game.gui << "\n\n";
    os << "Window size: " << game.window.getSize().x << "x" << game.window.getSize().y << "\n\n";
    os << "Current State: " << game.currentState << "\n\n";
    os << "Target Position: (" << game.target.x << ", " << game.target.y << ")\n";
    os << "####################################################################################################################################\n\n";
    return os;
}