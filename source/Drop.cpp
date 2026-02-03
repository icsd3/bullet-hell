#include "../headers/Drop.h"

Drop::Drop(const sf::Vector2f &pos, const sf::Texture &tex)
    : Object(pos, tex)
{
}

bool Drop::applyEffect(Player &player)
{
    return doApplyEffect(player);
}

void Drop::load()
{
    sf::Vector2f scaleFactor = {32.f, 0.f};
    sf::Vector2f spriteOriginFactor = {0.5f, 0.5f};
    sf::Vector2f collisionBoxSizeFactor = {0.6f, 0.6f}; 
    sf::Vector2f collisionBoxOriginFactor = {0.5f, 0.5f};
    sf::Vector2f collisionBoxPositionFactor = {0.f, 0.f};
    ObjectLoadParams params(scaleFactor, spriteOriginFactor, collisionBoxSizeFactor, collisionBoxOriginFactor, collisionBoxPositionFactor);
    Object::load(params);
}
