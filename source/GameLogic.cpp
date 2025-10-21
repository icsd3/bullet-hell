#include "../headers/GameLogic.h"
#include <iostream>

Game::Game()
    :m_window(sf::VideoMode::getDesktopMode(), "bullet hell game", sf::Style::Default, sf::State::Fullscreen)
{
    setUp();
}

Game& Game::getInstance()
{
    static Game instance;
    return instance;
}

void Game::setUp()
{
    loadBackground();
    m_window.setVerticalSyncEnabled(true);
}

void Game::loadBackground()
{
    if (!m_backgroundTexture.loadFromFile("textures/background.png"))
    {
        std::cerr << "Error loading background.png\n";
    }
    m_backgroundSprite.emplace(m_backgroundTexture);

    if(m_backgroundSprite)
    {
        const sf::Vector2u windowSize = m_window.getSize();
        const sf::Vector2u textureSize = m_backgroundTexture.getSize();
        m_backgroundSprite->setScale(sf::Vector2f(
            static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x),
            static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y)
        ));
    }
}

void Game::Play()
{
    while(m_window.isOpen()) {
        bool shouldExit = false;

        while(const std::optional event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                m_window.close();
                std::cout << "Fereastra a fost închisă\n";
            }
            else if (event->is<sf::Event::Resized>()) {
                std::cout << "New width: " << m_window.getSize().x << '\n'
                          << "New height: " << m_window.getSize().y << '\n';
            }
            else if (event->is<sf::Event::KeyPressed>()) {
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                std::cout << "Received key " << (keyPressed->scancode == sf::Keyboard::Scancode::X ? "X" : "(other)") << "\n";
                if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    shouldExit = true;
                }
            }
        }
        if(shouldExit) {
            m_window.close();
            std::cout << "Fereastra a fost închisă (shouldExit == true)\n";
            break;
        }

        m_window.clear();
        if(m_backgroundSprite) 
            m_window.draw(*m_backgroundSprite);
        m_window.display();
    }
}
