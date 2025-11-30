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

public:
    Entity(const bool &, const sf::Vector2f &, const bool &, const std::string &, const sf::Texture &, const float, const int);
    Entity(const Entity &);
    Entity &operator=(const Entity &);
    ~Entity() = default;
    friend std::ostream &operator<<(std::ostream &, const Entity &);

    void load() override;
    void draw(sf::RenderWindow &) override;

    virtual bool takeDamage(const int &);
    virtual int hits(const Entity &other) const;
};