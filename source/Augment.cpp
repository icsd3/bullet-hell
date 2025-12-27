#include "../headers/Augment.h"

Augment::Augment()
    : backgroundSprite(ResourceManager::getTexture(TextureType::AugmentBackground)),
      firstButtonSprite(ResourceManager::getTexture(TextureType::AugmentButton)),
      firstButtonText(ResourceManager::getFont(FontType::Courier)),
      secondButtonSprite(ResourceManager::getTexture(TextureType::AugmentButton)),
      secondButtonText(ResourceManager::getFont(FontType::Courier)),
      thirdButtonSprite(ResourceManager::getTexture(TextureType::AugmentButton)),
      thirdButtonText(ResourceManager::getFont(FontType::Courier))
{
}

void Augment::load()
{
    backgroundSprite.setScale(sf::Vector2f(
        1.f * LOGICAL_WIDTH / static_cast<float>(ResourceManager::getTexture(TextureType::AugmentBackground).getSize().x),
        1.f * LOGICAL_HEIGHT / static_cast<float>(ResourceManager::getTexture(TextureType::AugmentBackground).getSize().y)));

    float scaleX = 1.f * LOGICAL_WIDTH / static_cast<float>(ResourceManager::getTexture(TextureType::AugmentButton).getSize().x) / 13.f * 3.f;
    float scaleY = 1.f * LOGICAL_HEIGHT / static_cast<float>(ResourceManager::getTexture(TextureType::AugmentButton).getSize().y) / 10.f * 7.5f;
    auto setupButton = [&](sf::Sprite &sprite, int index)
    {
        sprite.setScale(sf::Vector2f(scaleX, scaleY));
        sprite.setOrigin(sf::Vector2f(static_cast<float>(ResourceManager::getTexture(TextureType::AugmentButton).getSize().x) / 2.f, static_cast<float>(ResourceManager::getTexture(TextureType::AugmentButton).getSize().y) / 2.f));
        sprite.setPosition(sf::Vector2f(LOGICAL_WIDTH / 26.f * (8 * index + 5), LOGICAL_HEIGHT / 2.f));
    };

    setupButton(firstButtonSprite, 0);
    setupButton(secondButtonSprite, 1);
    setupButton(thirdButtonSprite, 2);
}

void Augment::draw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    auto drawSprite = [&](const sf::Sprite &sprite)
    {
        sf::Sprite s = sprite;
        s.setPosition(Utils::mapToScreen(sprite.getPosition(), window));
        s.scale(scaleFactor);
        window.draw(s);
    };

    drawSprite(backgroundSprite);
    drawSprite(firstButtonSprite);
    drawSprite(secondButtonSprite);
    drawSprite(thirdButtonSprite);
}

int Augment::handleInput(const sf::RenderWindow &window, const sf::Event &event)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos = Utils::mapToLogical(sf::Vector2f(mouseEvent->position), window);

            if (firstButtonSprite.getGlobalBounds().contains(mousePos))
            {
                return 1;
            }
            if (secondButtonSprite.getGlobalBounds().contains(mousePos))
            {
                return 2;
            }
            if (thirdButtonSprite.getGlobalBounds().contains(mousePos))
            {
                return 3;
            }
        }
    }
    return 0;
}

// void Augment::applyModifiers(Player &player)
// {
//
// }
