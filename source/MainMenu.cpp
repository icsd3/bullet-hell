#include "../headers/MainMenu.h"
#include <iostream>

mainMenu::mainMenu()
    :backgroundPath("textures/menu_background.png"), 
    startButtonPath("textures/start_button.png"), 
    settingsButtonPath("textures/start_button.png"), 
    exitButtonPath("textures/exit_button.png"),
    backgroundTexture(backgroundPath),
    startButtonTexture(startButtonPath),
    settingsButtonTexture(settingsButtonPath),
    exitButtonTexture(exitButtonPath),
    backgroundSprite(backgroundTexture),
    startButtonSprite(startButtonTexture),
    settingsButtonSprite(settingsButtonTexture),
    exitButtonSprite(exitButtonTexture)
{
}

mainMenu &mainMenu::getInstance()
{
    static mainMenu instance;
    return instance;
}

void mainMenu::load(sf::RenderWindow &window)
{
    const sf::Vector2u windowSize = window.getSize();
    backgroundSprite.setScale(sf::Vector2f(
        1.f * windowSize.x / backgroundTexture.getSize().x,
        1.f * windowSize.y / backgroundTexture.getSize().y));

    startButtonSprite.setScale(sf::Vector2f(
        1.f * windowSize.x / startButtonTexture.getSize().x / 4.f,
        1.f * windowSize.x / startButtonTexture.getSize().x / 4.f));
    startButtonSprite.setOrigin(sf::Vector2f(
        startButtonTexture.getSize().x / 2.f,
        startButtonTexture.getSize().y / 2.f));
    startButtonSprite.setPosition(sf::Vector2f(
        windowSize.x / 2.f,
        windowSize.y / 4.f));

    settingsButtonSprite.setScale(sf::Vector2f(
        1.f * windowSize.x / settingsButtonTexture.getSize().x / 4.f,
        1.f * windowSize.x / settingsButtonTexture.getSize().x / 4.f));
    settingsButtonSprite.setOrigin(sf::Vector2f(
        settingsButtonTexture.getSize().x / 2.f,
        settingsButtonTexture.getSize().y / 2.f));
    settingsButtonSprite.setPosition(sf::Vector2f(
        windowSize.x / 2.f,
        windowSize.y / 4.f * 2.f));

    exitButtonSprite.setScale(sf::Vector2f(
        1.f * windowSize.x / exitButtonTexture.getSize().x / 4.f,
        1.f * windowSize.x / exitButtonTexture.getSize().x / 4.f));
    exitButtonSprite.setOrigin(sf::Vector2f(
        exitButtonTexture.getSize().x / 2.f,
        exitButtonTexture.getSize().y / 2.f));
    exitButtonSprite.setPosition(sf::Vector2f(
        windowSize.x / 2.f,
        windowSize.y / 4.f *3.f));
}

void mainMenu::draw(sf::RenderWindow &window)
{
    window.draw(backgroundSprite);
    window.draw(startButtonSprite);
    window.draw(settingsButtonSprite);
    window.draw(exitButtonSprite);
}

int mainMenu::handleInput(const sf::Event &event, const sf::RenderWindow &window)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            const auto startButtonBounds = startButtonSprite.getGlobalBounds();
            const auto settingsButtonBounds = settingsButtonSprite.getGlobalBounds();
            const auto exitButtonBounds = exitButtonSprite.getGlobalBounds();

            if (startButtonBounds.contains(sf::Vector2f(mouseEvent->position)))
            {
                std::cout << "Start button clicked!\n";
                return 1; // Start game
            }
            else if (settingsButtonBounds.contains(sf::Vector2f(mouseEvent->position)))
            {
                std::cout << "Settings button clicked!\n";
                return 2; // Open settings
            }
            else if (exitButtonBounds.contains(sf::Vector2f(mouseEvent->position)))
            {
                std::cout << "Exit button clicked!\n";
                return 3; // Exit game
            }
        }
    }
    else if (event.is<sf::Event::KeyPressed>())
    {
        const auto *keyboardEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyboardEvent->scancode == sf::Keyboard::Scancode::Enter)
        {
            std::cout << "Enter key pressed!\n";
            return 1; // Start game
        }
        else if(keyboardEvent->scancode == sf::Keyboard::Scancode::Escape)
        {
            std::cout << "Escape key pressed!\n";
            return 2; // Open settings
        }
    }
    return 0; // No action
}   


std::ostream &operator<<(std::ostream &os, const mainMenu &menu)
{
    os << "Main Menu:\n";
    os << "    Background Path: " << menu.backgroundPath << "\n";
    os << "    Start Button Path: " << menu.startButtonPath << "\n";
    os << "    Settings Button Path: " << menu.settingsButtonPath << "\n";
    os << "    Exit Button Path: " << menu.exitButtonPath << "\n";
    return os;
}