#include "../headers/Object.h"
#include <iostream>

Object::Object(const bool &pc, const sf::Vector2f &pos, const bool &ori, const std::string &tf, const sf::Texture &tex)
    : entityCollision(pc), position(pos), orientation(ori), textureFile(tf), sprite(tex), texture(tex)
{
}

Object::Object(const Object &other)
    : entityCollision(other.entityCollision), position(other.position), orientation(other.orientation), textureFile(other.textureFile), sprite(other.sprite), texture(other.texture), collisionBox(other.collisionBox)
{
}

Object &Object::operator=(const Object &other)
{
    if (this != &other)
    {
        entityCollision = other.entityCollision;
        position = other.position;
        orientation = other.orientation;
        textureFile = other.textureFile;
        sprite = other.sprite;
        collisionBox = other.collisionBox;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Object &object)
{
    os << "Object (Position: (" << object.position.x << ", " << object.position.y << "), Orientation: " << (object.orientation ? "right" : "left") << ", Texture: " << object.textureFile << ")";
    return os;
}

bool Object::collidesWith(const Object &other) const
{
    return (collisionBox.getGlobalBounds().findIntersection(other.collisionBox.getGlobalBounds())).has_value();
}

bool Object::collidesWith(const sf::RectangleShape &other) const
{
    return (collisionBox.getGlobalBounds().findIntersection(other.getGlobalBounds())).has_value();
}

void Object::draw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    sf::Sprite drawSprite = sprite;
    drawSprite.setPosition(Utils::mapToScreen(position, window));
    drawSprite.scale(scaleFactor);
    window.draw(drawSprite);

    if (Utils::changeDisplayBoxes(0) == 1)
    {
        sf::RectangleShape drawCollisionBox = collisionBox;
        drawCollisionBox.setPosition(Utils::mapToScreen(collisionBox.getPosition(), window));
        drawCollisionBox.scale(scaleFactor);
        window.draw(drawCollisionBox);
    }
}

void Object::load()
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    sprite.setPosition(position);
    
    if (orientation)
    {
        sprite.setScale(sf::Vector2f(-std::abs(sprite.getScale().x), sprite.getScale().y));
    }
    else
    {
        sprite.setScale(sf::Vector2f(std::abs(sprite.getScale().x), sprite.getScale().y));
    }

    collisionBox.setFillColor(sf::Color(0, 0, 255, 255));
}