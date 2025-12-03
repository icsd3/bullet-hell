#include "../headers/Object.h"
#include <iostream>

Object::Object(const sf::Vector2f &pos, const bool &ori, const sf::Texture &tex)
    : position(pos), orientation(ori), sprite(tex), texture(&tex)
{
}

Object::Object(const sf::Vector2f &pos, const bool &ori, const sf::Vector2f &size)
    : position(pos), orientation(ori), sprite(std::nullopt), texture(std::nullopt)
{
    collisionBox.setSize(size);
    collisionBox.setPosition(position);
}

Object::Object(const Object &other)
    : position(other.position), orientation(other.orientation), sprite(other.sprite), texture(other.texture), collisionBox(other.collisionBox)
{
}

Object &Object::operator=(const Object &other)
{
    if (this != &other)
    {
        position = other.position;
        orientation = other.orientation;
        sprite = other.sprite;
        collisionBox = other.collisionBox;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Object &object)
{
    os << "Object (Position: (" << object.position.x << ", " << object.position.y << "), Orientation: " << (object.orientation ? "right" : "left") << ")";
    return os;
}

bool Object::collidesWith(const Object &other) const
{
    return (collisionBox.getGlobalBounds().findIntersection(other.collisionBox.getGlobalBounds())).has_value();
}

void Object::doDraw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    if (sprite)
    {
        sf::Sprite drawSprite = *sprite;
        drawSprite.setPosition(Utils::mapToScreen(position, window));
        drawSprite.scale(scaleFactor);
        window.draw(drawSprite);
    }

    if (Utils::changeDisplayBoxes(0) == 1)
    {
        sf::RectangleShape drawCollisionBox = collisionBox;
        drawCollisionBox.setPosition(Utils::mapToScreen(collisionBox.getPosition(), window));
        drawCollisionBox.scale(scaleFactor);
        window.draw(drawCollisionBox);
    }
}

void Object::draw(sf::RenderWindow &window)
{
    doDraw(window);
}

void Object::doLoad()
{
    if (sprite)
    {
        sf::FloatRect bounds = sprite.value().getLocalBounds();
        sprite.value().setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
        sprite.value().setPosition(position);
        
        if (orientation)
        {
            sprite.value().setScale(sf::Vector2f(-std::abs(sprite.value().getScale().x), sprite.value().getScale().y));
        }
        else
        {
            sprite.value().setScale(sf::Vector2f(std::abs(sprite.value().getScale().x), sprite.value().getScale().y));
        }
    }

    collisionBox.setFillColor(sf::Color(0, 0, 200, 150));
}

void Object::load()
{
    doLoad();
}