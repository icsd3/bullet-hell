#pragma once

#include "../headers/Object.h"

#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

class Entity : public Object
{
protected:
    float speed;

public:
    Entity(const bool &, const sf::Vector2f &, const bool &, const std::string &, const sf::Texture &, float);
    Entity(const Entity &);
    Entity &operator=(const Entity &);
    ~Entity() = default;
    friend std::ostream &operator<<(std::ostream &, const Entity &);
};