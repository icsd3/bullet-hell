#pragma once

#include "../headers/Object.h"

class Door final : public Object
{
    int place;

public:
    Door(sf::Vector2f &, const sf::Texture &, const int &);
    void update(const unsigned int &);
    friend std::ostream &operator<<(std::ostream &, const Door &);

    void doorLoad(const sf::Vector2f &);
};