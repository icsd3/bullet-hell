#include "../headers/Door.h"

Door::Door(sf::Vector2f &pos, const int &plc)
    : Object(pos, (plc == 0 || plc == 2) ? TextureManager::getDoorVerticalTexture() : TextureManager::getDoorHorizontalTexture()), 
      place(plc)
{
}

void Door::doorLoad(const sf::Vector2f &size)
{
    Collider::load(size);
    collisionBox.setOrigin({size.x / 2, size.y / 2});
    collisionBox.setFillColor(sf::Color(200, 0, 0, 150));
    collisionBox.setPosition(position);

    sf::IntRect texRect({0, 0}, {300, 300});
    sprite.setTextureRect(texRect);

    if (place == 0)
    {
        sprite.setOrigin({150, 0});
        sprite.setPosition(sf::Vector2f(LOGICAL_WIDTH / 2, -66.5));
        position = {LOGICAL_WIDTH / 2, -66.5};
    }

    else if (place == 1)
    {
        sprite.setOrigin({300, 150});
        sprite.setPosition(sf::Vector2f(LOGICAL_WIDTH + 66.5, LOGICAL_HEIGHT / 2));
        position = {LOGICAL_WIDTH + 66.5, LOGICAL_HEIGHT / 2};
        collisionBox.setRotation(sf::Angle(sf::degrees(90)));
    }

    else if (place == 2)
    {
        sprite.setOrigin({150, 0});
        sprite.setPosition(sf::Vector2f(LOGICAL_WIDTH / 2, LOGICAL_HEIGHT + 66.5));
        sprite.setScale({1, -1});
        position = {LOGICAL_WIDTH / 2, LOGICAL_HEIGHT + 66.5};
    }

    else if (place == 3)
    {
        sprite.setOrigin({300, 150});
        sprite.setPosition(sf::Vector2f(-66.5, LOGICAL_HEIGHT / 2));
        sprite.setScale({-1, 1});
        position = {-66.5, LOGICAL_HEIGHT / 2};
        collisionBox.setRotation(sf::Angle(sf::degrees(90)));
    }
}

void Door::update(const unsigned int &frame)
{
    sprite.setTextureRect(sf::IntRect({(int)frame * 300, 0}, {300, 300}));
    if (frame * 300 + 300 == 1500)
        collisionBox.setFillColor(sf::Color(0, 0, 200, 150));
}

std::ostream &operator<<(std::ostream &os, const Door &door)
{
    os << "Door (Position: (" << door.position.x << ", " << door.position.y << ")" << ", Place: " << door.place << ")";
    return os;
}