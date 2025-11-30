#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include "../headers/Utils.h"

class Object
{
protected:
    bool entityCollision;
    sf::Vector2f position;
    bool orientation = false; // false = left, true = right
    bool isEntity = false;

    std::string textureFile;
    sf::Sprite sprite;
    const sf::Texture &texture;

    sf::RectangleShape collisionBox;

public:
    Object(const bool &, const sf::Vector2f &, const bool &, const std::string &, const sf::Texture &);
    Object(const Object &);
    Object &operator=(const Object &);
    ~Object() = default;
    friend std::ostream &operator<<(std::ostream &, const Object &);

    bool collidesWith(const Object &) const;
    bool collidesWith(const sf::RectangleShape &) const;

    virtual void draw(sf::RenderWindow &);
    virtual void load();
};