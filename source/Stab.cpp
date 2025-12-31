#include "../headers/Stab.h"

Stab::Stab(const sf::Vector2f &pos, sf::Texture &tex, const int &dmg, const sf::Vector2f &dir, const float &rn, const float &dur, const float &spd)
    : Attack(pos, tex, 0, 10, dmg, dir, rn), duration(dur), stabSpeed(spd)
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

    Entity::load({range, 0.f}, {0.5f, 0.5f}, {0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, 4, {
        {0.5f, 0.f},
        {1.f, 0.5f},
        {0.5f, 1.f},
        {0.f, 0.5f}
    });

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
