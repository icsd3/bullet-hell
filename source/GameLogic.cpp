#include "../headers/GameLogic.h"
#include <iostream>

Game::Game()
    :m_window(sf::VideoMode::getDesktopMode(), "BulletHell", sf::Style::Default, sf::State::Fullscreen)
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
    m_window.setVerticalSyncEnabled(true);
}

void Game::loadMainMenu()
{
    if (!m_menuBackgroundTexture.loadFromFile("textures/menu_background.png"))
    {
        std::cerr << "Error loading menu_background.png\n";
    }
    m_menuBackgroundSprite.emplace(m_menuBackgroundTexture);

    if(m_menuBackgroundSprite)
    {
        const sf::Vector2u windowSize = m_window.getSize();
        const sf::Vector2u textureSize = m_menuBackgroundTexture.getSize();
        m_menuBackgroundSprite->setScale(sf::Vector2f(
            static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x),
            static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y)
        ));
    }

    if (!m_startButtonTexture.loadFromFile("textures/start_button.png"))
    {
        std::cerr << "Error loading start_button.png\n";
    }
    m_startButtonSprite.emplace(m_startButtonTexture);

    if(m_startButtonSprite)
    {
        const sf::Vector2u windowSize = m_window.getSize();
        const sf::Vector2u startButtonSize = m_startButtonTexture.getSize();
        const float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(startButtonSize.x) /4.f;
        const float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(startButtonSize.y) /2.25f;
        m_startButtonSprite->setScale(sf::Vector2f(scaleX, scaleY));
        m_startButtonSprite->setPosition(sf::Vector2f(
            (windowSize.x - startButtonSize.x * scaleX) / 2.f,
            (windowSize.y - startButtonSize.y * scaleY) / 2.f
        ));
    }
}

void Game::loadAugment()
{
    if (!m_augmentBackgroundTexture.loadFromFile("textures/augment_background.png"))
    {
        std::cerr << "Error loading augment_background.png\n";
    }
    m_augmentBackgroundSprite.emplace(m_augmentBackgroundTexture);

    if(m_augmentBackgroundSprite)
    {
        const sf::Vector2u windowSize = m_window.getSize();
        const sf::Vector2u textureSize = m_augmentBackgroundTexture.getSize();
        m_augmentBackgroundSprite->setScale(sf::Vector2f(
            static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x),
            static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y)
        ));
    }
}

void Game::loadLevel()
{
    
}

void Game::handleMainMenuInput(const sf::Event& event)
{
    if(event.is<sf::Event::MouseButtonPressed>())
    {
        if (!m_startButtonSprite) 
            return;

        auto mousePos = sf::Mouse::getPosition(m_window);
        const auto bounds = m_startButtonSprite->getGlobalBounds();

        if (bounds.contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))) 
        {
            std::cout << "Start button clicked!\n";
            Progress::selectGameState(reinterpret_cast<int&>(m_currentState));
            handleNewState(m_currentState);
        }
    }
    else
    {
        std::cout << "Enter key pressed!\n";
        Progress::selectGameState(reinterpret_cast<int&>(m_currentState));
        handleNewState(m_currentState);
    }
}

void Game::handleNewState(gameStates newState)
{
    m_currentState = newState;

    switch (m_currentState)
    {
        case menu:
            loadMainMenu();
            std::cout << "menu\n";
            break;
        case augment_1:
            loadAugment();
            std::cout << "augment_1\n";
            break;
        case level_1:
            std::cout << "level_1\n";
            break;
        default:
            break;
    }
}

bool Game::handleInputs()
{
    bool shouldExit = false;

        while(const std::optional event = m_window.pollEvent()) 
        {
            if (event->is<sf::Event::Closed>()) 
            {
                m_window.close();
                std::cout << "Fereastra a fost închisă\n";
            }
            else if (event->is<sf::Event::KeyPressed>()) 
            {
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                std::cout << "Received key " << (keyPressed->scancode == sf::Keyboard::Scancode::X ? "X" : "(other)") << "\n";
                if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) 
                {
                    shouldExit = true;
                }
                else if(keyPressed->scancode == sf::Keyboard::Scancode::Enter) 
                {
                    if (m_currentState == menu)
                        handleMainMenuInput(*event);
                }
            }
            else if (event->is<sf::Event::MouseButtonPressed>())
            {
                const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseEvent->button == sf::Mouse::Button::Left)
                {
                    if (m_currentState == menu)
                        handleMainMenuInput(*event);
                }
            }
        }
        return shouldExit;
}

void Game::drawMenu()
{
    if(m_menuBackgroundSprite) 
        m_window.draw(*m_menuBackgroundSprite);
    if(m_startButtonSprite)
        m_window.draw(*m_startButtonSprite);
}

void Game::drawAugment()
{
    if(m_augmentBackgroundSprite) 
        m_window.draw(*m_augmentBackgroundSprite);
}

void Game::drawLevel()
{
    m_window.clear(sf::Color::Green);
}

void Game::drawVictory()
{
    m_window.clear(sf::Color::Red);
}

void Game::draw()
{
    m_window.clear();
        
        switch (m_currentState)
        {
            case menu:
            case game_over:
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

            case victory:
                drawVictory();
                break;

            default:
                break;
        }

    m_window.display();
}

void Game::Play()
{
    while(m_window.isOpen()) {
        bool shouldExit = handleInputs();

        if(shouldExit) 
        {
            m_window.close();
            std::cout << "Fereastra a fost închisă (shouldExit == true)\n";
            break;
        }

        draw();
    }
}
