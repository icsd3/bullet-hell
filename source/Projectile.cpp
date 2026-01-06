#include "../headers/Projectile.h"

Projectile::Projectile(const sf::Vector2f &pos, sf::Texture &tex, float spd, const int &pierce, const int &dmg, const sf::Vector2f &dir, const float &rn)
    : Attack(pos, tex, spd, pierce, dmg, dir, rn)
{
}

void Projectile::doLoad()
{
    sf::Angle rotation = sf::degrees(0);

    if (direction != sf::Vector2f(0, 0))
        rotation = direction.angle();

    sf::Vector2f scaleFactor = {35.f, 0.f};
    sf::Vector2f spriteOriginFactor = {0.5f, 0.5f};
    sf::Vector2f collisionBoxSizeFactor = {0.5f, 0.5f}; // relative to sprite dimensions
    sf::Vector2f collisionBoxOriginFactor = {0.5f, 0.5f}; // relative to collision box
    sf::Vector2f collisionBoxPositionFactor = {0.f, 0.f}; // relative to sprite
    int hitBoxPointCount = 8;
    std::vector<sf::Vector2f> hitBoxPointFactors = { // relative to sprite dimensions (0,0) is top-left, (1,1) is bottom-right
        {3.5f / 20, 0.f},
        {16.5f / 20, 0.f},
        {1.f, 3.5f / 10},
        {1.f, 6.5f / 10},
        {16.5f / 20, 1.f},
        {3.5f / 20, 1.f},
        {0.f, 6.5f / 10},
        {0.f, 3.5f / 10}
    };
    ObjectLoadParams params(scaleFactor, spriteOriginFactor, collisionBoxSizeFactor, collisionBoxOriginFactor, collisionBoxPositionFactor);

    Entity::load(params, hitBoxPointCount, hitBoxPointFactors);

    transform({0.f, 0.f}, false, rotation);
}

bool Projectile::doUpdate(const float &dt)
{
    sf::Vector2f moveVec(direction * speed * dt);
    transform(moveVec, false, sf::Angle(sf::degrees(0.f)));
    
    sf::Vector2f dir = position - origin;
    float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (distance > range)
        return true;

    return false;
}

std::ostream &operator<<(std::ostream &os, const Projectile &projectile)
{
    os << static_cast<const Attack &>(projectile);
    return os;
}