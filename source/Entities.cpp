#include "../headers/Entities.h"
#include <iostream>

Object::Object(const sf::Vector2f& pos, const bool& ori, const std::string& textureFile)
    :position(pos), orientation(ori)
{
    loadObject(textureFile);
}

Object::Object(const Object& other)
    :position(other.position), orientation(other.orientation), texture(other.texture)
{
    if(other.sprite)
    {
        sprite.emplace(texture);
    }
}

Object& Object::operator=(const Object& other)
{
    if (this != &other)
    {
        position = other.position;
        orientation = other.orientation;
        texture = other.texture;
        if(other.sprite)
        {
            sprite.emplace(texture);
        }
        else
        {
            sprite.reset();
        }
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Object& object)
{
    os << "Object(Position: (" << object.position.x << ", " << object.position.y << "), Angle: " << object.orientation << ")";
    return os;
}

void Object::loadObject(const std::string& textureFile)
{
    if (!texture.loadFromFile(textureFile))
    {
        std::cerr << "Error loading texture: " << textureFile << "\n";
    }
    sprite.emplace(texture);
}

Entity::Entity(const sf::Vector2f& pos, const bool& ori, const std::string& textureFile, float spd)
    :Object(pos, ori, textureFile), speed(spd)
{

}

Entity::Entity(const Entity& other)
    :Object(other), speed(other.speed)
{

}

Entity& Entity::operator=(const Entity& other)
{
    if (this != &other)
    {
        Object::operator=(other);
        speed = other.speed;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Entity& entity)
{
    os << "Entity(";
    os << static_cast<const Object&>(entity);
    os << ", Speed: " << entity.speed << ")";
    return os;
}