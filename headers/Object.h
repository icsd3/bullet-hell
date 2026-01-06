#pragma once

#include "../headers/Collider.h"
#include "../headers/ResourceManager.h"

struct ObjectLoadParams
{
    ObjectLoadParams(
        const sf::Vector2f &sf,
        const sf::Vector2f &sof,
        const sf::Vector2f &cbsf,
        const sf::Vector2f &cbof,
        const sf::Vector2f &cbpf)
        : scaleFactor(sf), spriteOriginFactor(sof), collisionBoxSizeFactor(cbsf), collisionBoxOriginFactor(cbof), collisionBoxPositionFactor(cbpf)
    {
    }
    const sf::Vector2f &scaleFactor;
    const sf::Vector2f &spriteOriginFactor;
    const sf::Vector2f &collisionBoxSizeFactor;
    const sf::Vector2f &collisionBoxOriginFactor;
    const sf::Vector2f &collisionBoxPositionFactor;
};

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

    void load(const ObjectLoadParams &);
};