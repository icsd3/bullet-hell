#include "../headers/Door.h"

Door::Door(sf::Vector2f &pos, bool &ori, const sf::Texture &tex, sf::Vector2f &size, const int &plc)
    : Object(pos, ori, tex), place(plc)
{
    collisionBox.setPointCount(4);
    collisionBox.setPoint(0, sf::Vector2f(0.f, 0.f));
    collisionBox.setPoint(1, sf::Vector2f(size.x, 0.f));
    collisionBox.setPoint(2, sf::Vector2f(size.x, size.y));
    collisionBox.setPoint(3, sf::Vector2f(0.f, size.y));
    collisionBox.setOrigin({size.x / 2, size.y / 2});
}

void Door::doLoad()
{
    collisionBox.setFillColor(sf::Color(200, 0, 0, 150));
    collisionBox.setPosition(position);

    sf::IntRect texRect({0, 0}, {300, 300});
    sprite.value().setTextureRect(texRect);

    if (place == 0)
    {
        sprite.value().setOrigin({150, 0});
        sprite.value().setPosition(sf::Vector2f(LOGICAL_WIDTH / 2, -66.5));
        position = {LOGICAL_WIDTH / 2, -66.5};
    }

    else if (place == 1)
    {
        sprite.value().setOrigin({300, 150});
        sprite.value().setPosition(sf::Vector2f(LOGICAL_WIDTH + 66.5, LOGICAL_HEIGHT / 2));
        position = {LOGICAL_WIDTH + 66.5, LOGICAL_HEIGHT / 2};
        collisionBox.setRotation(sf::Angle(sf::degrees(90)));
    }

    else if (place == 2)
    {
        sprite.value().setOrigin({150, 0});
        sprite.value().setPosition(sf::Vector2f(LOGICAL_WIDTH / 2, LOGICAL_HEIGHT + 66.5));
        sprite.value().setScale({1, -1});
        position = {LOGICAL_WIDTH / 2, LOGICAL_HEIGHT + 66.5};
    }

    else if (place == 3)
    {
        sprite.value().setOrigin({300, 150});
        sprite.value().setPosition(sf::Vector2f(-66.5, LOGICAL_HEIGHT / 2));
        sprite.value().setScale({-1, 1});
        position = {-66.5, LOGICAL_HEIGHT / 2};
        collisionBox.setRotation(sf::Angle(sf::degrees(90)));
    }
}

void Door::update(const unsigned int &frame)
{
    sprite.value().setTextureRect(sf::IntRect({(int)frame * 300, 0}, {300, 300}));
    if (frame * 300 + 300 == texture.value()->getSize().x)
        collisionBox.setFillColor(sf::Color(0, 0, 200, 150));
}

std::ostream &operator<<(std::ostream &os, const Door &door)
{
    os << "Door (Position: (" << door.position.x << ", " << door.position.y << "), Orientation: " << (door.orientation ? "right" : "left") << ", Place: " << door.place << ")";
    return os;
}