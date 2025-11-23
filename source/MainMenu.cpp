#include "../headers/MainMenu.h"
#include "../headers/Utils.h"
#include <iostream>

mainMenu::mainMenu()
    :backgroundPath("textures/menu_background.png"), 
    startButtonPath("textures/start_button.png"), 
    settingsButtonPath("textures/settings_button.png"), 
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

void mainMenu::load()
{
    backgroundSprite.setScale(sf::Vector2f(
        1.f * LOGICAL_WIDTH / backgroundTexture.getSize().x,
        1.f * LOGICAL_HEIGHT / backgroundTexture.getSize().y));

    startButtonSprite.setScale(sf::Vector2f(
        1.f * LOGICAL_WIDTH / startButtonTexture.getSize().x / 4.f,
        1.f * LOGICAL_WIDTH / startButtonTexture.getSize().x / 4.f));
    startButtonSprite.setOrigin(sf::Vector2f(
        startButtonTexture.getSize().x / 2.f,
        startButtonTexture.getSize().y / 2.f));
    startButtonSprite.setPosition(sf::Vector2f(
        LOGICAL_WIDTH / 2.f,
        LOGICAL_HEIGHT / 4.f));

    settingsButtonSprite.setScale(sf::Vector2f(
        1.f * LOGICAL_WIDTH / settingsButtonTexture.getSize().x / 4.f,
        1.f * LOGICAL_WIDTH / settingsButtonTexture.getSize().x / 4.f));
    settingsButtonSprite.setOrigin(sf::Vector2f(
        settingsButtonTexture.getSize().x / 2.f,
        settingsButtonTexture.getSize().y / 2.f));
    settingsButtonSprite.setPosition(sf::Vector2f(
        LOGICAL_WIDTH / 2.f,
        LOGICAL_HEIGHT / 4.f * 2.f));

    exitButtonSprite.setScale(sf::Vector2f(
        1.f * LOGICAL_WIDTH / exitButtonTexture.getSize().x / 4.f,
        1.f * LOGICAL_WIDTH / exitButtonTexture.getSize().x / 4.f));
    exitButtonSprite.setOrigin(sf::Vector2f(
        exitButtonTexture.getSize().x / 2.f,
        exitButtonTexture.getSize().y / 2.f));
    exitButtonSprite.setPosition(sf::Vector2f(
        LOGICAL_WIDTH / 2.f,
        LOGICAL_HEIGHT / 4.f *3.f));
}

void mainMenu::draw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = getScaleFactor(window);

    sf::Sprite drawBackground = backgroundSprite;
    drawBackground.setPosition(mapToScreen(backgroundSprite.getPosition(), window));
    drawBackground.scale(scaleFactor);
    window.draw(drawBackground);

    sf::Sprite drawStart = startButtonSprite;
    drawStart.setPosition(mapToScreen(startButtonSprite.getPosition(), window));
    drawStart.scale(scaleFactor);
    window.draw(drawStart);

    sf::Sprite drawSettings = settingsButtonSprite;
    drawSettings.setPosition(mapToScreen(settingsButtonSprite.getPosition(), window));
    drawSettings.scale(scaleFactor);
    window.draw(drawSettings);

    sf::Sprite drawExit = exitButtonSprite;
    drawExit.setPosition(mapToScreen(exitButtonSprite.getPosition(), window));
    drawExit.scale(scaleFactor);
    window.draw(drawExit);
}

int mainMenu::handleInput(const sf::RenderWindow &window, const sf::Event &event)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos = mapToLogical(sf::Vector2f(mouseEvent->position), window);

            const auto startButtonBounds = startButtonSprite.getGlobalBounds();
            const auto settingsButtonBounds = settingsButtonSprite.getGlobalBounds();
            const auto exitButtonBounds = exitButtonSprite.getGlobalBounds();

            if (startButtonBounds.contains(mousePos))
            {
                return 1; // Start game
            }
            else if (settingsButtonBounds.contains(mousePos))
            {
                return 2; // Open settings
            }
            else if (exitButtonBounds.contains(mousePos))
            {
                return 3; // Exit game
            }
        }
    }
    else if (event.is<sf::Event::KeyPressed>())
    {
        const auto *keyboardEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyboardEvent->scancode == sf::Keyboard::Scancode::Enter)
        {
            return 1; // Start game
        }
        else if (keyboardEvent->scancode == sf::Keyboard::Scancode::Escape)
        {
            return 2; // Open settings
        }
    }
    return 0;
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