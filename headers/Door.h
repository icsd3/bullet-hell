#pragma once

#include "../headers/Object.h"

class Door : public Object
{
    int place;

public:
    Door(sf::Vector2f &, const sf::Texture &, sf::Vector2f &, const int &);
    void update(const unsigned int &);
    friend std::ostream &operator<<(std::ostream &, const Door &);

    void load();
};