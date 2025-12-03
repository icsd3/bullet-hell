#pragma once

#include "../headers/Object.h"

#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

class Entity : public Object
{
protected:
    float speed;
    int maxHealth;
    int currentHealth;
    sf::ConvexShape hitBox;
    // sf::Clock animationClock;

    void doLoad() override;
    void doDraw(sf::RenderWindow &) override;

    virtual bool doTakeDamage(const int &);
    virtual int doHits(const Entity &other) const;

public:
    Entity(const sf::Vector2f &, const bool &, sf::Texture &, const float, const int);
    Entity(const Entity &);
    Entity &operator=(const Entity &);
    friend std::ostream &operator<<(std::ostream &, const Entity &);

    int hits(const Entity &other) const;
    bool takeDamage(const int &);
};