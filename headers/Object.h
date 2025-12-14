#pragma once

#include "../headers/Collider.h"

class Object : public Collider
{
protected:
    sf::Sprite sprite;
    const sf::Texture* texture;    

    void doDraw(sf::RenderWindow &) override;

public:
    Object(const sf::Vector2f &, const sf::Texture &);
    Object(const Object &);
    Object &operator=(const Object &);
    friend std::ostream &operator<<(std::ostream &, const Object &);

    void load(const float &, const sf::Vector2f &, const sf::Vector2f &, const sf::Vector2f &);
};