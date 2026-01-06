#include "../headers/Slash.h"

Slash::Slash(const sf::Vector2f &pos, sf::Texture &tex, const int &dmg, const sf::Vector2f &dir, const float &rn, const float &dur, const float &ang)
    : Attack(pos, tex, 0, 10, dmg, dir, rn), duration(dur), arcAngle(sf::degrees(ang))
{
}

Slash::Slash(const Slash &other)
    : Attack(other), duration(other.duration), arcAngle(other.arcAngle)
{
}

Slash &Slash::operator=(const Slash &other)
{
    if (this != &other)
    {
        Attack::operator=(other);
        duration = other.duration;
        arcAngle = other.arcAngle;
    }
    return *this;
}

void Slash::doLoad()
{
    sf::Angle angle = sf::degrees(90);

    if (direction != sf::Vector2f(0, 0))
        angle += direction.angle();

    sf::Vector2f scaleFactor = {range * 3 / 2, 0.f};
    sf::Vector2f spriteOriginFactor = {0.5f, 0.5f};
    sf::Vector2f collisionBoxSizeFactor = {0.5f, 0.2f}; // relative to sprite dimensions
    sf::Vector2f collisionBoxOriginFactor = {0.5f, -1.f}; // relative to collision box
    sf::Vector2f collisionBoxPositionFactor = {0.f, 0.f}; // relative to sprite
    int hitBoxPointCount = 7;
    std::vector<sf::Vector2f> hitBoxPointFactors = { // relative to sprite dimensions (0,0) is top-left, (1,1) is bottom-right
        {0.5f, 0.f},
        {1.f / 6, 1.f / 6},
        {0.f, 0.5f},
        {0.f, 1.f},
        {1.f, 1.f},
        {1.f, 0.5f},
        {5.f / 6, 1.f / 6}
    };
    ObjectLoadParams params(scaleFactor, spriteOriginFactor, collisionBoxSizeFactor, collisionBoxOriginFactor, collisionBoxPositionFactor);

    Entity::load(params, hitBoxPointCount, hitBoxPointFactors);

    bool facingLeft = false;

    if (angle > sf::degrees(180) || angle < sf::degrees(0))
        facingLeft = true;

    transform({0.f, 0.f}, facingLeft, angle);
}

bool Slash::doUpdate(const float &dt)
{
    duration -= dt;
    return duration <= 0.f;
}

std::ostream &operator<<(std::ostream &os, const Slash &slash)
{
    os << static_cast<const Attack &>(slash);
    os << "\n";
    os << "        Lifetime: " << slash.duration << "\n";
    return os;
}
