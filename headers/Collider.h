#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

#include "../headers/Utils.h"

class Collider
{
protected:
    sf::Vector2f position;
    sf::ConvexShape collisionBox;

    virtual void doDraw(sf::RenderWindow &);

public:
    explicit Collider(const sf::Vector2f &);
    Collider(const Collider &);
    Collider &operator=(const Collider &);
    virtual ~Collider() = default;
    friend std::ostream &operator<<(std::ostream &, const Collider &);

    bool collidesWith(const Collider &) const;
    bool collidesWith(const sf::ConvexShape &) const;

    void draw(sf::RenderWindow &);
    void load(const sf::Vector2f &);
};
