#include "../headers/Stab.h"

Stab::Stab(const sf::Vector2f &pos, sf::Texture &tex, const int &dmg, const sf::Vector2f &dir, const float &rn, const float &dur, const float &spd)
    : Attack(pos, tex, 0, 5, dmg, dir, rn), duration(dur), stabSpeed(spd)
{
}

Stab::Stab(const Stab &other)
    : Attack(other), duration(other.duration), stabSpeed(other.stabSpeed)
{
}

Stab &Stab::operator=(const Stab &other)
{
    if (this != &other)
    {
        Attack::operator=(other);
        duration = other.duration;
        stabSpeed = other.stabSpeed;
    }
    return *this;
}

void Stab::doLoad()
{
    sf::Angle angle = sf::degrees(0);

    if (direction != sf::Vector2f(0, 0))
        angle += direction.angle();

    sf::Vector2f scaleFactor = {range, 0.f};
    sf::Vector2f spriteOriginFactor = {0.5f, 0.5f};
    sf::Vector2f collisionBoxSizeFactor = {0.4f, 0.2f}; // relative to sprite dimensions
    sf::Vector2f collisionBoxOriginFactor = {0.75f, 0.5f}; // relative to collision box
    sf::Vector2f collisionBoxPositionFactor = {0.f, 0.f}; // relative to sprite
    int hitBoxPointCount = 4;
    std::vector<sf::Vector2f> hitBoxPointFactors = { // relative to sprite dimensions (0,0) is top-left, (1,1) is bottom-right
        {0.5f, 0.f},
        {1.f, 0.5f},
        {0.5f, 1.f},
        {0.f, 0.5f}
    };
    ObjectLoadParams params(scaleFactor, spriteOriginFactor, collisionBoxSizeFactor, collisionBoxOriginFactor, collisionBoxPositionFactor);

    Entity::load(params, hitBoxPointCount, hitBoxPointFactors);

    transform({0.f, 0.f}, false, angle);
}

bool Stab::doUpdate(const float &dt)
{
    transform((stabSpeed * direction * dt), false, sf::degrees(0));

    duration -= dt;
    return duration <= 0.f;
}

std::ostream &operator<<(std::ostream &os, const Stab &stab)
{
    os << static_cast<const Attack &>(stab);
    os << "\n";
    os << "        Lifetime: " << stab.duration << "\n";
    return os;
}
