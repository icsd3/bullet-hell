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

    Entity::load({35.f, 0.f}, {0.5f, 0.5f}, {0.5f, 0.5f}, {0.5f, 0.5f}, {0.f, 0.f}, 8, {
        {3.5f / 20, 0.f},
        {16.5f / 20, 0.f},
        {1.f, 3.5f / 10},
        {1.f, 6.5f / 10},
        {16.5f / 20, 1.f},
        {3.5f / 20, 1.f},
        {0.f, 6.5f / 10},
        {0.f, 3.5f / 10}
    });

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