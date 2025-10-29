#include "../headers/GameLogic.h"
#include <iostream>

Game::Game()
    :window(sf::VideoMode::getDesktopMode(), "BulletHell", sf::Style::Default, sf::State::Fullscreen)
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
            player.position = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);
            target = player.position;
            player.loadPlayer();
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

        while(const std::optional event = window.pollEvent()) 
        {
            if (event->is<sf::Event::Closed>()) 
            {
                window.close();
                std::cout << "Fereastra a fost inchisa fortat\n";
            }
            else if (event->is<sf::Event::KeyPressed>()) 
            {
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                if(keyPressed->scancode == sf::Keyboard::Scancode::Enter) 
                {
                    if (currentState == menu)
                        handleMainMenuInput(*event);
                }
                if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) 
                {
                    shouldExit=true;
                    std::cout<<"Escape pressed bye bye!\n";
                }
            }
            else if (event->is<sf::Event::MouseButtonPressed>())
            {
                const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseEvent->button == sf::Mouse::Button::Left)
                {
                    if (currentState == menu)
                        shouldExit = handleMainMenuInput(*event);
                    else if(currentState == augment_1 || currentState == augment_2 || currentState == augment_3)
                    {
                        handleAugmentInput(*event);
                    }
                }
                else if (mouseEvent->button == sf::Mouse::Button::Right)
                {
                    if(currentState == level_1 || currentState == level_2 || currentState == level_3)
                    {
                        handleLevelInput(*event);
                    }
                }
            }
        }
        return shouldExit;
}

bool Game::handleMainMenuInput(const sf::Event& event)
{
    if(event.is<sf::Event::MouseButtonPressed>())
    {
        if (!loader.menuButtonSprite[0] || !loader.menuButtonSprite[1]) 
            return false;

        auto mousePos = sf::Mouse::getPosition(window);
        const auto startButtonBounds = loader.menuButtonSprite[0]->getGlobalBounds();
        const auto exitButtonBounds = loader.menuButtonSprite[1]->getGlobalBounds();

        if (startButtonBounds.contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))) 
        {
            std::cout << "Start button clicked!\n";
            Progress::selectGameState(currentState);
            handleNewState();
        }
        else if(exitButtonBounds.contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))))
        {
            std::cout << "Exit button clicked!\n";
            return true;
        }
    }
    else
    {
        std::cout << "Enter key pressed!\n";
        Progress::selectGameState(currentState);
        handleNewState();
    }
    return false;
}

void Game::handleAugmentInput(const sf::Event& event)
{
    if(event.is<sf::Event::MouseButtonPressed>())
    {
        if (!loader.augmentButtonSprite[0] || !loader.augmentButtonSprite[1] || !loader.augmentButtonSprite[2]) 
            return;

        auto mousePos = sf::Mouse::getPosition(window);
        for(int i=0; i<3; i++)
        {
            const auto buttonBounds = loader.augmentButtonSprite[i]->getGlobalBounds();

            if (buttonBounds.contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))) 
            {
                std::cout << "Augment button " << i+1 << " clicked!\n";
                Progress::selectGameState(currentState);
                handleNewState();
                break;
            }
        }
    }
}

void Game::handleLevelInput(const sf::Event& event)
{
    if(event.is<sf::Event::MouseButtonPressed>())
    {
        const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

        if (!player.sprite) 
            return;

        if(mouseEvent->button == sf::Mouse::Button::Right)
        {
            target = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        }
    }
}

void Game::drawMenu()
{
    if(loader.menuBackgroundSprite) 
        window.draw(*loader.menuBackgroundSprite);
    if(loader.menuButtonSprite[0])
        window.draw(*loader.menuButtonSprite[0]);
    if(loader.menuButtonSprite[1])
        window.draw(*loader.menuButtonSprite[1]);
}

void Game::drawAugment()
{
    if(loader.augmentBackgroundSprite) 
        window.draw(*loader.augmentBackgroundSprite);
    for(int i=0; i<3; i++)
        if(loader.augmentButtonSprite[i])
            window.draw(*(loader.augmentButtonSprite[i]));
}

void Game::drawLevel()
{
    if(loader.levelBackgroundSprite) 
        window.draw(*loader.levelBackgroundSprite);
    if(player.sprite)
        window.draw(*player.sprite);
    drawGUI();
}

void Game::drawGUI()
{
    gui.updateGUI(player.currentHealth, player.maxHealth);
    window.draw(gui.health);
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
                drawMenu();
                break;

            case augment_1:
            case augment_2:
            case augment_3:
                drawAugment();
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
            player.updatePlayer(clock, target);
        draw();
    }
}

std::ostream& operator<<(std::ostream& os, const Game& game)
{
    os << "Current Game State: " << game.currentState << "\n";
    return os;
}