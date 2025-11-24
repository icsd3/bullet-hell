#include "../headers/Room.h"
#include "../headers/Utils.h"

Room::Room()
    :backgroundPath("textures/level_background.png"),
    backgroundTexture(backgroundPath),
    backgroundSprite(backgroundTexture)
{
    load();
}

void Room::load()
{
    backgroundSprite.setScale(sf::Vector2f(
        1.f * LOGICAL_WIDTH / backgroundTexture.getSize().x,
        1.f * LOGICAL_HEIGHT / backgroundTexture.getSize().y));
}

void Room::draw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = getScaleFactor(window);
    sf::Sprite drawBg = backgroundSprite;
    drawBg.setPosition(mapToScreen(backgroundSprite.getPosition(), window));
    drawBg.scale(scaleFactor);
    window.draw(drawBg);
}