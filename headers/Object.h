#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

class Object
{
protected:
    bool entityCollision;
    sf::Vector2f position;
    bool orientation = false; // false = left, true = right
    bool isEntity = false;

    std::string textureFile;
    sf::Sprite sprite;

public:
    bool collidesWith(const Object &) const;

    Object(const bool &ec, const sf::Vector2f &, const bool &, const std::string &, const sf::Texture &);
    Object(const Object &);
    Object &operator=(const Object &);
    ~Object() = default;
    friend std::ostream &operator<<(std::ostream &, const Object &);
};