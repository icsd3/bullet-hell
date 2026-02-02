#include "../headers/PauseMenu.h"

PauseMenu::PauseMenu()
    : resumeButtonSprite(ResourceManager::getTexture(TextureType::PauseResumeButton)),
      settingsButtonSprite(ResourceManager::getTexture(TextureType::MenuSettingsButton)),
      mainMenuButtonSprite(ResourceManager::getTexture(TextureType::PauseMenuButton))
{
}

void PauseMenu::load()
{
    pauseBox.setSize(sf::Vector2f(0.3f * LOGICAL_WIDTH, 0.8f * LOGICAL_HEIGHT));
    pauseBox.setOrigin(sf::Vector2f(0.5f * pauseBox.getLocalBounds().size.x, 0.5f * pauseBox.getLocalBounds().size.y));
    pauseBox.setPosition(sf::Vector2f(0.5f * LOGICAL_WIDTH, 0.5f * LOGICAL_HEIGHT));
    pauseBox.setFillColor(sf::Color(0, 0, 0, 150));
    pauseBox.setOutlineThickness(10.f);
    pauseBox.setOutlineColor(sf::Color::Black);

    resumeButtonSprite.setScale(sf::Vector2f(
        1.f * LOGICAL_WIDTH / ResourceManager::getTexture(TextureType::PauseResumeButton).getSize().x / 4.f,
        1.f * LOGICAL_WIDTH / ResourceManager::getTexture(TextureType::PauseResumeButton).getSize().x / 4.f));
    resumeButtonSprite.setOrigin(sf::Vector2f(
        ResourceManager::getTexture(TextureType::PauseResumeButton).getSize().x / 2.f,
        ResourceManager::getTexture(TextureType::PauseResumeButton).getSize().y / 2.f));
    resumeButtonSprite.setPosition(sf::Vector2f(
        LOGICAL_WIDTH / 2.f,
        LOGICAL_HEIGHT / 4.f));

    settingsButtonSprite.setScale(sf::Vector2f(
        1.f * LOGICAL_WIDTH / ResourceManager::getTexture(TextureType::MenuSettingsButton).getSize().x / 4.f,
        1.f * LOGICAL_WIDTH / ResourceManager::getTexture(TextureType::MenuSettingsButton).getSize().x / 4.f));
    settingsButtonSprite.setOrigin(sf::Vector2f(
        ResourceManager::getTexture(TextureType::MenuSettingsButton).getSize().x / 2.f,
        ResourceManager::getTexture(TextureType::MenuSettingsButton).getSize().y / 2.f));
    settingsButtonSprite.setPosition(sf::Vector2f(
        LOGICAL_WIDTH / 2.f,
        LOGICAL_HEIGHT / 4.f * 2.f));

    mainMenuButtonSprite.setScale(sf::Vector2f(
        1.f * LOGICAL_WIDTH / ResourceManager::getTexture(TextureType::PauseMenuButton).getSize().x / 4.f,
        1.f * LOGICAL_WIDTH / ResourceManager::getTexture(TextureType::PauseMenuButton).getSize().x / 4.f));
    mainMenuButtonSprite.setOrigin(sf::Vector2f(
        ResourceManager::getTexture(TextureType::PauseMenuButton).getSize().x / 2.f,
        ResourceManager::getTexture(TextureType::PauseMenuButton).getSize().y / 2.f));
    mainMenuButtonSprite.setPosition(sf::Vector2f(
        LOGICAL_WIDTH / 2.f,
        LOGICAL_HEIGHT / 4.f * 3.f));
}

void PauseMenu::draw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    sf::RectangleShape drawBox = pauseBox;
    drawBox.setPosition(Utils::mapToScreen(pauseBox.getPosition(), window));
    drawBox.setSize(sf::Vector2f(pauseBox.getSize().x * scaleFactor.x, pauseBox.getSize().y * scaleFactor.y));
    drawBox.setOrigin(sf::Vector2f(drawBox.getSize().x / 2.f, drawBox.getSize().y / 2.f));
    drawBox.setOutlineThickness(pauseBox.getOutlineThickness() * scaleFactor.x);
    window.draw(drawBox);

    sf::Sprite drawResume = resumeButtonSprite;
    drawResume.setPosition(Utils::mapToScreen(resumeButtonSprite.getPosition(), window));
    drawResume.scale(scaleFactor);
    window.draw(drawResume);

    sf::Sprite drawSettings = settingsButtonSprite;
    drawSettings.setPosition(Utils::mapToScreen(settingsButtonSprite.getPosition(), window));
    drawSettings.scale(scaleFactor);
    window.draw(drawSettings);

    sf::Sprite drawMainMenu = mainMenuButtonSprite;
    drawMainMenu.setPosition(Utils::mapToScreen(mainMenuButtonSprite.getPosition(), window));
    drawMainMenu.scale(scaleFactor);
    window.draw(drawMainMenu);
}

int PauseMenu::handleInput(const sf::RenderWindow &window, const sf::Event &event)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos = Utils::mapToLogical(sf::Vector2f(mouseEvent->position), window);

            const auto resumeButtonBounds = resumeButtonSprite.getGlobalBounds();
            const auto settingsButtonBounds = settingsButtonSprite.getGlobalBounds();
            const auto mainMenuButtonBounds = mainMenuButtonSprite.getGlobalBounds();

            if (resumeButtonBounds.contains(mousePos))
            {
                return 1; // Resume
            }
            else if (settingsButtonBounds.contains(mousePos))
            {
                return 2; // Settings
            }
            else if (mainMenuButtonBounds.contains(mousePos))
            {
                return 3; // Main Menu
            }
        }
    }
    else if (event.is<sf::Event::KeyPressed>())
    {
        const auto *keyboardEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyboardEvent->scancode == sf::Keyboard::Scancode::Escape)
        {
            return 1; // Resume
        }
    }
    return 0;
}
