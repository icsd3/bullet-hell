#include "../headers/Object.h"
#include <iostream>

Object::Object(const bool &pc, const sf::Vector2f &pos, const bool &ori, const std::string &tf, const sf::Texture &tex)
    : entityCollision(pc), position(pos), orientation(ori), textureFile(tf), sprite(tex)
{
}

Object::Object(const Object &other)
    : entityCollision(other.entityCollision), position(other.position), orientation(other.orientation), textureFile(other.textureFile), sprite(other.sprite)
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
    return (sprite.getGlobalBounds().findIntersection(other.sprite.getGlobalBounds())).has_value();
}

bool Object::collidesWith(const sf::RectangleShape &other) const
{
    return (sprite.getGlobalBounds().findIntersection(other.getGlobalBounds())).has_value();
}