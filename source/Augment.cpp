#include "../headers/Augment.h"

Augment::Augment()
    : backgroundPath("textures/augment_background.png"),
      buttonPath("textures/augment_button.png"),
      fontPath("fonts/courier.ttf"),
      backgroundTexture(backgroundPath),
      buttonTexture(buttonPath),
      font(fontPath),
      backgroundSprite(backgroundTexture),
      firstButtonSprite(buttonTexture),
      firstButtonText(font),
      secondButtonSprite(buttonTexture),
      secondButtonText(font),
      thirdButtonSprite(buttonTexture),
      thirdButtonText(font)
{
}

Augment &Augment::getInstance()
{
    static Augment instance;
    return instance;
}

void Augment::load()
{
    backgroundSprite.setScale(sf::Vector2f(
        1.f * LOGICAL_WIDTH / static_cast<float>(backgroundTexture.getSize().x),
        1.f * LOGICAL_HEIGHT / static_cast<float>(backgroundTexture.getSize().y)));

    float scaleX = 1.f * LOGICAL_WIDTH / static_cast<float>(buttonTexture.getSize().x) / 13.f * 3.f;
    float scaleY = 1.f * LOGICAL_HEIGHT / static_cast<float>(buttonTexture.getSize().y) / 10.f * 7.5f;

    auto setupButton = [&](sf::Sprite &sprite, int index)
    {
        sprite.setScale(sf::Vector2f(scaleX, scaleY));
        sprite.setOrigin(sf::Vector2f(static_cast<float>(buttonTexture.getSize().x) / 2.f, static_cast<float>(buttonTexture.getSize().y) / 2.f));
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

std::ostream &operator<<(std::ostream &os, const Augment &augment)
{
    os << "Augment:\n";
    os << "    Background Path: " << augment.backgroundPath << "\n";
    os << "    Button Path: " << augment.buttonPath << "\n";
    os << "    Font Path: " << augment.fontPath << "\n";
    return os;
}
