#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <optional>
#include <iostream>

class Object
{
    friend class Game;

protected:
    sf::Vector2f position;
    sf::Angle angle;

    sf::Texture texture;
    std::optional<sf::Sprite> sprite;

    void loadObject(const std::string& textureFile);

public:
    Object(const sf::Vector2f& pos, sf::Angle ang, const std::string& textureFile);
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
    Entity(const sf::Vector2f& pos, sf::Angle ang, const std::string& textureFile, float spd);
    Entity(const Entity&);
    Entity& operator=(const Entity&);
    ~Entity() = default;
    friend std::ostream& operator<<(std::ostream& os, const Entity& entity);
};