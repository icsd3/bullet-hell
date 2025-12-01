#pragma once

#include "../headers/Object.h"

class Door : public Object
{
    int place;
    
public:
    Door(sf::Vector2f &, bool &, const sf::Texture &, sf::Vector2f &, const int &);
    void load() override;
    void update(const int &);
    friend std::ostream &operator<<(std::ostream &, const Door &);
};