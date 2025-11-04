#include "../headers/Entities.h"
#include <iostream>

Object::Object(const bool& pc, const sf::Vector2f& pos, const bool& ori, const std::string& tf)
    :entityCollision(pc), position(pos), orientation(ori), textureFile(tf), texture(textureFile), sprite(texture)
{
    
}

Object::Object(const Object& other)
    :entityCollision(other.entityCollision), position(other.position), orientation(other.orientation), textureFile(other.textureFile), texture(textureFile), sprite(texture)
{
    texture = sf::Texture("textures/player.png");
    sprite.setTexture(texture);
    sprite.setOrigin(other.sprite.getOrigin());
    sprite.setPosition(other.sprite.getPosition());
    sprite.setScale(other.sprite.getScale());
    sprite.setRotation(other.sprite.getRotation());
}

Object& Object::operator=(const Object& other)
{
    if (this != &other)
    {
        entityCollision = other.entityCollision;
        position = other.position;
        orientation = other.orientation;
        textureFile = other.textureFile;
        texture = other.texture;
        sprite = other.sprite;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Object& object)
{
    os << "Object (Position: (" << object.position.x << ", " << object.position.y << "), Orientation: " << (object.orientation ? "right" : "left") << ", Texture: " << object.textureFile << ")";
    return os;
}

Entity::Entity(const bool& ec, const sf::Vector2f& pos, const bool& ori, const std::string& tf, float spd)
    :Object(ec, pos, ori, tf), speed(spd)
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
    os << "    Entity (";
    os << static_cast<const Object&>(entity);
    os << ", Speed: " << entity.speed << ")";
    return os;
}