#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <optional>
#include <iostream>

class Object
{
    bool entityCollision;

protected:
    sf::Vector2f position;
    bool orientation = false; // false = left, true = right
    bool isEntity=false;

    std::string textureFile;
    sf::Sprite sprite;

public:
    bool collidesWith(const Object& other) const;

    Object(const bool& ec, const sf::Vector2f& pos, const bool& ori, const std::string& tf, const sf::Texture& tex);
    Object(const Object&);
    Object& operator=(const Object&);
    ~Object() = default;
    friend std::ostream& operator<<(std::ostream& os, const Object& object);
};

class Entity : public Object
{

protected:
    float speed;

public:
    Entity(const bool& ec, const sf::Vector2f& pos, const bool& ori, const std::string& tf, const sf::Texture& tex, float spd);
    Entity(const Entity&);
    Entity& operator=(const Entity&);
    ~Entity() = default;
    friend std::ostream& operator<<(std::ostream& os, const Entity& entity);
};