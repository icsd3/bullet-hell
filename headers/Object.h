#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include "../headers/Utils.h"

class Object
{
protected:
    sf::Vector2f position;
    bool orientation = false; // false = left, true = right

    std::optional<sf::Sprite> sprite;
    std::optional<const sf::Texture*> texture;    

    sf::ConvexShape collisionBox;

    virtual void doDraw(sf::RenderWindow &);
    virtual void doLoad();

public:
    Object(const sf::Vector2f &, const bool &, const sf::Texture &);
    Object(const sf::Vector2f &, const bool &, const sf::Vector2f &);
    Object(const Object &);
    Object &operator=(const Object &);
    virtual ~Object() = default;
    friend std::ostream &operator<<(std::ostream &, const Object &);

    bool collidesWith(const Object &) const;
    bool lineIntersects(const sf::Vector2f &, const sf::Vector2f &) const;

    void draw(sf::RenderWindow &);
    void load();
};