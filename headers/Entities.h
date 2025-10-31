#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <optional>
#include <iostream>

class Object
{
    friend class Game;
    bool entityCollision;

protected:
    sf::Vector2f position;
    bool orientation = false; // false = left, true = right

    sf::Texture texture;
    std::optional<sf::Sprite> sprite;

    void loadObject(const std::string& textureFile);

public:
    Object(const bool& ec, const sf::Vector2f& pos, const bool& ori, const std::string& textureFile);
    Object(const Object&);
    Object& operator=(const Object&);
    ~Object() = default;
    friend std::ostream& operator<<(std::ostream& os, const Object& object);
};

class Entity : public Object
{
    friend class Game;

protected:
    float speed;

public:
    Entity(const bool& ec, const sf::Vector2f& pos, const bool& ori, const std::string& textureFile, float spd);
    Entity(const Entity&);
    Entity& operator=(const Entity&);
    ~Entity() = default;
    friend std::ostream& operator<<(std::ostream& os, const Entity& entity);
};