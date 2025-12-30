#include "../headers/Slash.h"

Slash::Slash(const sf::Vector2f &pos, sf::Texture &tex, const int &dmg, const sf::Vector2f &dir, const float &rn, const float &dur, const float &ang)
    : Attack(pos, tex, 0, 10, dmg, dir, rn), duration(dur), arcAngle(sf::degrees(ang))
{
}

void Slash::doLoad()
{
    sf::Angle angle = sf::degrees(90);

    if (direction != sf::Vector2f(0, 0))
        angle += direction.angle();

    Entity::load(range * 3 / 2, {0.5f, 0.2f}, {0.5f, -1.f}, {0.f, 0.f}, 7, {
        {0.5f, 0.f},
        {1.f / 6, 1.f / 6},
        {0.f, 0.5f},
        {0.f, 1.f},
        {1.f, 1.f},
        {1.f, 0.5f},
        {5.f / 6, 1.f / 6}
    });

    bool facingLeft = false;

    if (angle > sf::degrees(180) || angle < sf::degrees(0))
        facingLeft = true;

    transform({0.f, 0.f}, facingLeft, angle);
}

bool Slash::doUpdate(const float &dt)
{
    duration -= dt;

    if (duration <= 0.f)
        return true;

    return false;
}

std::ostream &operator<<(std::ostream &os, const Slash &slash)
{
    os << static_cast<const Attack &>(slash);
    os << "\n";
    os << "        Lifetime: " << slash.duration << "\n";
    return os;
}
