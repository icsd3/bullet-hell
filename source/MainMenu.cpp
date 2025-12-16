#include "../headers/MainMenu.h"

mainMenu::mainMenu()
    : backgroundSprite(TextureManager::getMenuBackgroundTexture()),
      startButtonSprite(TextureManager::getMenuStartButtonTexture()),
      settingsButtonSprite(TextureManager::getMenuSettingsButtonTexture()),
      exitButtonSprite(TextureManager::getMenuExitButtonTexture())
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
        1.f * LOGICAL_WIDTH / static_cast<float>(TextureManager::getMenuBackgroundTexture().getSize().x),
        1.f * LOGICAL_HEIGHT / static_cast<float>(TextureManager::getMenuBackgroundTexture().getSize().y)));

    startButtonSprite.setScale(sf::Vector2f(
        1.f * LOGICAL_WIDTH / TextureManager::getMenuStartButtonTexture().getSize().x / 4.f,
        1.f * LOGICAL_WIDTH / TextureManager::getMenuStartButtonTexture().getSize().x / 4.f));
    startButtonSprite.setOrigin(sf::Vector2f(
        TextureManager::getMenuStartButtonTexture().getSize().x / 2.f,
        TextureManager::getMenuStartButtonTexture().getSize().y / 2.f));
    startButtonSprite.setPosition(sf::Vector2f(
        LOGICAL_WIDTH / 2.f,
        LOGICAL_HEIGHT / 4.f));

    settingsButtonSprite.setScale(sf::Vector2f(
        1.f * LOGICAL_WIDTH / TextureManager::getMenuSettingsButtonTexture().getSize().x / 4.f,
        1.f * LOGICAL_WIDTH / TextureManager::getMenuSettingsButtonTexture().getSize().x / 4.f));
    settingsButtonSprite.setOrigin(sf::Vector2f(
        TextureManager::getMenuSettingsButtonTexture().getSize().x / 2.f,
        TextureManager::getMenuSettingsButtonTexture().getSize().y / 2.f));
    settingsButtonSprite.setPosition(sf::Vector2f(
        LOGICAL_WIDTH / 2.f,
        LOGICAL_HEIGHT / 4.f * 2.f));

    exitButtonSprite.setScale(sf::Vector2f(
        1.f * LOGICAL_WIDTH / TextureManager::getMenuExitButtonTexture().getSize().x / 4.f,
        1.f * LOGICAL_WIDTH / TextureManager::getMenuExitButtonTexture().getSize().x / 4.f));
    exitButtonSprite.setOrigin(sf::Vector2f(
        TextureManager::getMenuExitButtonTexture().getSize().x / 2.f,
        TextureManager::getMenuExitButtonTexture().getSize().y / 2.f));
    exitButtonSprite.setPosition(sf::Vector2f(
        LOGICAL_WIDTH / 2.f,
        LOGICAL_HEIGHT / 4.f * 3.f));
}

void mainMenu::draw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    sf::Sprite drawBackground = backgroundSprite;
    drawBackground.setPosition(Utils::mapToScreen(backgroundSprite.getPosition(), window));
    drawBackground.scale(scaleFactor);
    window.draw(drawBackground);

    sf::Sprite drawStart = startButtonSprite;
    drawStart.setPosition(Utils::mapToScreen(startButtonSprite.getPosition(), window));
    drawStart.scale(scaleFactor);
    window.draw(drawStart);

    sf::Sprite drawSettings = settingsButtonSprite;
    drawSettings.setPosition(Utils::mapToScreen(settingsButtonSprite.getPosition(), window));
    drawSettings.scale(scaleFactor);
    window.draw(drawSettings);

    sf::Sprite drawExit = exitButtonSprite;
    drawExit.setPosition(Utils::mapToScreen(exitButtonSprite.getPosition(), window));
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
            sf::Vector2f mousePos = Utils::mapToLogical(sf::Vector2f(mouseEvent->position), window);

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