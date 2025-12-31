#include "../headers/GameOverMenu.h"

GameOverMenu::GameOverMenu()
    : titleText(ResourceManager::getFont(FontType::Courier)),
      mainMenuButtonSprite(ResourceManager::getTexture(TextureType::PauseMenuButton))
{
}

GameOverMenu &GameOverMenu::getInstance()
{
    static GameOverMenu instance;
    return instance;
}

void GameOverMenu::load(bool victory)
{
    backgroundBox.setSize(sf::Vector2f(0.4f * LOGICAL_WIDTH, 0.4f * LOGICAL_HEIGHT));
    backgroundBox.setOrigin(sf::Vector2f(0.5f * backgroundBox.getLocalBounds().size.x, 0.5f * backgroundBox.getLocalBounds().size.y));
    backgroundBox.setPosition(sf::Vector2f(0.5f * LOGICAL_WIDTH, 0.5f * LOGICAL_HEIGHT));
    backgroundBox.setOutlineThickness(10.f);
    backgroundBox.setFillColor(sf::Color(0, 0, 0, 150));

    titleText.setCharacterSize(80);
    
    if (victory)
    {
        titleText.setString("VICTORY");
        titleText.setFillColor(sf::Color(0, 150, 0, 255));
        backgroundBox.setOutlineColor(sf::Color(0, 150, 0, 255));
    }
    else
    {
        titleText.setString("YOU DIED");
        titleText.setFillColor(sf::Color(150, 0, 0, 255));
        backgroundBox.setOutlineColor(sf::Color(150, 0, 0, 255));
    }

    titleText.setOrigin(sf::Vector2f(titleText.getLocalBounds().position.x + 0.5f * titleText.getLocalBounds().size.x,
                                     titleText.getLocalBounds().position.y + 0.5f * titleText.getLocalBounds().size.y));
    titleText.setPosition(sf::Vector2f(backgroundBox.getPosition().x, backgroundBox.getPosition().y - backgroundBox.getSize().y / 4.f));

    mainMenuButtonSprite.setScale(sf::Vector2f(
        1.f * LOGICAL_WIDTH / ResourceManager::getTexture(TextureType::PauseMenuButton).getSize().x / 4.f,
        1.f * LOGICAL_WIDTH / ResourceManager::getTexture(TextureType::PauseMenuButton).getSize().x / 4.f));
    mainMenuButtonSprite.setOrigin(sf::Vector2f(
        ResourceManager::getTexture(TextureType::PauseMenuButton).getSize().x / 2.f,
        ResourceManager::getTexture(TextureType::PauseMenuButton).getSize().y / 2.f));
    mainMenuButtonSprite.setPosition(sf::Vector2f(
        backgroundBox.getPosition().x,
        backgroundBox.getPosition().y + backgroundBox.getSize().y / 5.f));
}

void GameOverMenu::draw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    sf::RectangleShape drawBox = backgroundBox;
    drawBox.setPosition(Utils::mapToScreen(backgroundBox.getPosition(), window));
    drawBox.setSize(sf::Vector2f(backgroundBox.getSize().x * scaleFactor.x, backgroundBox.getSize().y * scaleFactor.y));
    drawBox.setOrigin(sf::Vector2f(drawBox.getSize().x / 2.f, drawBox.getSize().y / 2.f));
    drawBox.setOutlineThickness(backgroundBox.getOutlineThickness() * scaleFactor.x);
    window.draw(drawBox);

    sf::Text drawTitle = titleText;
    drawTitle.setPosition(Utils::mapToScreen(titleText.getPosition(), window));
    drawTitle.setScale(scaleFactor);
    drawTitle.setOrigin(sf::Vector2f(drawTitle.getLocalBounds().position.x + 0.5f * drawTitle.getLocalBounds().size.x,
                                     drawTitle.getLocalBounds().position.y + 0.5f * drawTitle.getLocalBounds().size.y));
    window.draw(drawTitle);

    sf::Sprite drawMainMenu = mainMenuButtonSprite;
    drawMainMenu.setPosition(Utils::mapToScreen(mainMenuButtonSprite.getPosition(), window));
    drawMainMenu.scale(scaleFactor);
    window.draw(drawMainMenu);
}

int GameOverMenu::handleInput(const sf::RenderWindow &window, const sf::Event &event)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos = Utils::mapToLogical(sf::Vector2f(mouseEvent->position), window);

            const auto mainMenuButtonBounds = mainMenuButtonSprite.getGlobalBounds();

            if (mainMenuButtonBounds.contains(mousePos))
            {
                return 1; // Main Menu
            }
        }
    }
    return 0;
}
