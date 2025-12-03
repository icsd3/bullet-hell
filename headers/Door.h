#pragma once

#include "../headers/Object.h"

class Door : public Object
{
    int place;
    void doLoad() override;
    
public:
    Door(sf::Vector2f &, bool &, const sf::Texture &, sf::Vector2f &, const int &);
    void update(const unsigned int &);
    friend std::ostream &operator<<(std::ostream &, const Door &);
};