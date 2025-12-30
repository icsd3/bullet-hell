#pragma once

#include "../headers/Collider.h"
#include "../headers/ResourceManager.h"

class Object : public Collider
{
    const sf::Texture *texture;

protected:
    sf::Sprite sprite;

    void doDraw(sf::RenderWindow &) const override;

public:
    Object(const sf::Vector2f &, const sf::Texture &);
    Object(const Object &);
    Object &operator=(const Object &);
    friend std::ostream &operator<<(std::ostream &, const Object &);

    void load(const float &, const sf::Vector2f &, const sf::Vector2f &, const sf::Vector2f &);
};