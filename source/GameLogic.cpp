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
    handleNewState(menu);
    window.setVerticalSyncEnabled(true);
}

void Game::loadMainMenu()
{
    if (!menuBackgroundTexture.loadFromFile("textures/menu_background.png"))
    {
        std::cerr << "Error loading menu_background.png\n";
    }
    menuBackgroundSprite.emplace(menuBackgroundTexture);

    if(menuBackgroundSprite)
    {
        const sf::Vector2u windowSize = window.getSize();
        const sf::Vector2u textureSize = menuBackgroundTexture.getSize();
        menuBackgroundSprite->setScale(sf::Vector2f(
            static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x),
            static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y)
        ));
    }
    for(int i = 0; i<2; i++)
    {
        if(i==0)
        {
            if (!menuButtonTexture[i].loadFromFile("textures/start_button.png"))
            {
                std::cerr << "Error loading start_button.png\n";
            }
        }
        else
        {
            if (!menuButtonTexture[i].loadFromFile("textures/exit_button.png"))
            {
                std::cerr << "Error loading exit_button.png\n";
            }
        }
        menuButtonSprite[i].emplace(menuButtonTexture[i]);

        if(menuButtonSprite[i])
        {
            const sf::Vector2u windowSize = window.getSize();
            const sf::Vector2u startButtonSize = menuButtonTexture[i].getSize();
            const float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(startButtonSize.x) /4.f;
            const float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(startButtonSize.y) /4.5f;
            menuButtonSprite[i]->setScale(sf::Vector2f(scaleX, scaleY));
            menuButtonSprite[i]->setPosition(sf::Vector2f(
                (windowSize.x - startButtonSize.x * scaleX) / 2.f,
                (windowSize.y - startButtonSize.y * scaleY) / 4.f * (2*i+1)
            ));
        }
    }
    
}

void Game::loadAugment()
{
    if (!augmentBackgroundTexture.loadFromFile("textures/augment_background.png"))
    {
        std::cerr << "Error loading augment_background.png\n";
    }
    augmentBackgroundSprite.emplace(augmentBackgroundTexture);

    if(augmentBackgroundSprite)
    {
        const sf::Vector2u windowSize = window.getSize();
        const sf::Vector2u textureSize = augmentBackgroundTexture.getSize();
        augmentBackgroundSprite->setScale(sf::Vector2f(
            static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x),
            static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y)
        ));
    }
}

void Game::loadLevel()
{
    
}

void Game::loadDefeat()
{

}

void Game::loadVictory()
{

}

bool Game::handleMainMenuInput(const sf::Event& event)
{
    if(event.is<sf::Event::MouseButtonPressed>())
    {
        if (!menuButtonSprite[0] || !menuButtonSprite[1]) 
            return false;

        auto mousePos = sf::Mouse::getPosition(window);
        const auto startButtonBounds = menuButtonSprite[0]->getGlobalBounds();
        const auto exitButtonBounds = menuButtonSprite[1]->getGlobalBounds();

        if (startButtonBounds.contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))) 
        {
            std::cout << "Start button clicked!\n";
            Progress::selectGameState(reinterpret_cast<int&>(currentState));
            handleNewState(currentState);
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
        Progress::selectGameState(reinterpret_cast<int&>(currentState));
        handleNewState(currentState);
    }
    return false;
}

void Game::handleNewState(gameStates newState)
{
    currentState = newState;

    switch (currentState)
    {
        case menu:
            loadMainMenu();
            std::cout << "menu\n";
            break;
        case augment_1:
            loadAugment();
            std::cout << "augment_1\n";
            break;
        case augment_2:
            loadAugment();
            std::cout << "augment_2\n";
            break;
        case augment_3:
            loadAugment();
            std::cout << "augment_3\n";
            break;
        case level_1:
            loadLevel();
            std::cout << "level_1\n";
            break;
        case level_2:
            loadLevel();
            std::cout << "level_2\n";
            break;
        case level_3:
            loadLevel();
            std::cout << "level_3\n";
            break;
        case defeat:
            loadDefeat();
            std::cout << "defeat\n";
            break;
        case victory:
            loadVictory();
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
                std::cout << "Fereastra a fost închisă fortat\n";
            }
            else if (event->is<sf::Event::KeyPressed>()) 
            {
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                if(keyPressed->scancode == sf::Keyboard::Scancode::Enter) 
                {
                    if (currentState == menu)
                        handleMainMenuInput(*event);
                }
            }
            else if (event->is<sf::Event::MouseButtonPressed>())
            {
                const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseEvent->button == sf::Mouse::Button::Left)
                {
                    if (currentState == menu)
                        shouldExit = handleMainMenuInput(*event);
                }
            }
        }
        return shouldExit;
}

void Game::drawMenu()
{
    if(menuBackgroundSprite) 
        window.draw(*menuBackgroundSprite);
    if(menuButtonSprite[0])
        window.draw(*menuButtonSprite[0]);
    if(menuButtonSprite[1])
        window.draw(*menuButtonSprite[1]);
}

void Game::drawAugment()
{
    if(augmentBackgroundSprite) 
        window.draw(*augmentBackgroundSprite);
}

void Game::drawLevel()
{
    window.clear(sf::Color::Green);
}

void Game::drawDefeat()
{

}

void Game::drawVictory()
{
    
}

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
                drawDefeat();
                break;

            case victory:
                drawVictory();
                break;

            default:
                break;
        }

    window.display();
}

void Game::Play()
{
    while(window.isOpen()) {
        bool shouldExit = handleInputs();

        if(shouldExit) 
        {
            window.close();
            std::cout << "Fereastra a fost închisă (shouldExit == true)\n";
            break;
        }

        draw();
    }
}