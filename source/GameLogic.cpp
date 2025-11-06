#include "../headers/GameLogic.h"
#include <iostream>

Game::Game()
    :window(sf::VideoMode::getDesktopMode(), "BulletHell", sf::Style::Default, sf::State::Fullscreen),
    target(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y * 0.8f))
{
    setup();
}

Game& Game::getInstance() 
{
    static Game instance;
    return instance;
}

void Game::setup()
{
    window.clear(sf::Color::Black);
    window.display();
    loader.loadStaticAssets();
    currentState = menu;
    handleNewState();
    window.setVerticalSyncEnabled(true);
}

void Game::handleNewState()
{
    switch (currentState)
    {
        case menu:
            loader.loadMainMenu(window);
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
            player.loadPlayer(window);
            gui.loadGUI();
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

bool Game::handleInputs()
{
    bool shouldExit = false;

    if(currentState == level_1 || currentState == level_2 || currentState == level_3)
    {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        {
            target = sf::Vector2f(sf::Mouse::getPosition());
        }
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            float dt = weaponClock.getElapsedTime().asSeconds();
            if(player.canFireCurrentWeapon(dt))
            {
                weaponClock.restart();
                sf::Vector2f projectileTarget = sf::Vector2f(sf::Mouse::getPosition());
                playerProjectiles.push_back(player.fireCurrentWeapon(projectileTarget));
                playerProjectiles.back().loadProjectile(window);
            }
        }
    }

    while(const std::optional event = window.pollEvent()) 
    {
        if (event->is<sf::Event::Closed>()) 
        {
            window.close();
            std::cout << "Fereastra a fost inchisa fortat\n";
        }
        else if (event->is<sf::Event::KeyPressed>() || event->is<sf::Event::MouseButtonPressed>()) 
        {
            if (event->is<sf::Event::KeyPressed>())
            {
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                if(keyPressed->scancode == sf::Keyboard::Scancode::Backspace) 
                {
                    shouldExit=true;
                    std::cout<<"Backspace pressed bye bye!\n";
                }
                else if(keyPressed->scancode == sf::Keyboard::Scancode::I)
                {
                    std::cout << *this;
                }
            }
            else if (currentState == menu)
                shouldExit = handleMainMenuInput(*event);
            else if (currentState == augment_1 || currentState == augment_2 || currentState == augment_3)
                handleAugmentInput(*event);
            else if (currentState == level_1 || currentState == level_2 || currentState == level_3)
                handleLevelInput(*event);
        }
    }
    return shouldExit;
}

bool Game::handleMainMenuInput(const sf::Event& event)
{
    if(event.is<sf::Event::MouseButtonPressed>())
    {
        const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

        if(mouseEvent->button == sf::Mouse::Button::Left)
        {
            if (!loader.hasMenuButtonSprites()) 
                return false;

            const auto startButtonBounds = loader.getMenuButtonBounds(0);
            const auto exitButtonBounds = loader.getMenuButtonBounds(1);

            if (startButtonBounds.contains(sf::Vector2f(mouseEvent->position))) 
            {
                std::cout << "Start button clicked!\n";
                Progress::selectGameState(currentState);
                handleNewState();
            }
            else if(exitButtonBounds.contains(sf::Vector2f(mouseEvent->position)))
            {
                std::cout << "Exit button clicked!\n";
                return true;
            }
        }
    }
    else if(event.is<sf::Event::KeyPressed>())
    {
        const auto* keyboardEvent = event.getIf<sf::Event::KeyPressed>();
        if(keyboardEvent->scancode == sf::Keyboard::Scancode::Enter)
        {
            std::cout << "Enter key pressed!\n";
            Progress::selectGameState(currentState);
            handleNewState();
        }
    }
    return false;
}

void Game::handleAugmentInput(const sf::Event& event)
{
    if(event.is<sf::Event::MouseButtonPressed>())
    {
        const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

        if(mouseEvent->button == sf::Mouse::Button::Left)
        {
            if (!loader.hasAugmentButtonSprites()) 
            return;

            for(int i=0; i<3; i++)
            {
                const auto buttonBounds = loader.getAugmentButtonBounds(i);

                if (buttonBounds.contains(sf::Vector2f(mouseEvent->position)))
                {
                    std::cout << "Augment button " << i+1 << " clicked!\n";
                    Progress::selectGameState(currentState);
                    handleNewState();
                    break;
                }
            }
        }
    }
}

void Game::handleLevelInput(const sf::Event& event)
{
    if(event.is<sf::Event::MouseButtonPressed>())
    {
        const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

        if(mouseEvent->button == sf::Mouse::Button::Right)
        {
            target = sf::Vector2f(mouseEvent->position);
        }

        if(mouseEvent->button == sf::Mouse::Button::Left)
        {
            float dt = weaponClock.getElapsedTime().asSeconds();
            if(player.canFireCurrentWeapon(dt))
            {
                weaponClock.restart();
                sf::Vector2f projectileTarget = sf::Vector2f(mouseEvent->position);
                playerProjectiles.push_back(player.fireCurrentWeapon(projectileTarget));
                playerProjectiles.back().loadProjectile(window);
                std::cout<<"fired\n";
            }
        }
    }
    if(event.is<sf::Event::KeyPressed>())
    {
        // const auto* keyBoardEvent = event.getIf<sf::Event::KeyPressed>();
    }
}

void Game::drawLevel()
{
    loader.drawLevelBackground(window);
    player.drawPlayer(window);
    for(auto& projectile : playerProjectiles)
    {
        projectile.drawProjectile(window);
    }
    drawGUI();
}

void Game::drawGUI()
{
    gui.updateGUI(player.getHealthStatus());
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
            case menu:
                loader.drawMenu(window);
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

void Game::Play()
{
    while(window.isOpen()) 
    {
        bool shouldExit = handleInputs();

        if(shouldExit) 
        {
            window.close();
            std::cout << "Fereastra a fost inchisa (shouldExit == true)\n";
            break;
        }
        if(currentState == level_1 || currentState == level_2 || currentState == level_3)
        {
            float dt = updateClock.restart().asSeconds();
            player.updatePlayer(dt, target);
            for(size_t i = 0; i < playerProjectiles.size(); )
            {
                if(playerProjectiles[i].updateProjectile(dt, window))
                    playerProjectiles.erase(playerProjectiles.begin() + i);
                else
                    i++;
            }
        }
        draw();
    }
}

std::ostream& operator<<(std::ostream& os, const Game& game)
{
    os << "\n####################################################################################################################################\n";
    os << "Loader status:\n" << game.loader << "\n\n"; 
    os << "Player status:\n" << game.player << "\n\n";
    os << "GUI status:\n" << game.gui << "\n\n";
    os << "Window size: " << game.window.getSize().x << "x" << game.window.getSize().y << "\n\n";
    os << "Current State: " << game.currentState << "\n\n";
    os << "Target Position: (" << game.target.x << ", " << game.target.y << ")\n";
    os << "####################################################################################################################################\n\n";
    return os;
}