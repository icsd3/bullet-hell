#include "../headers/Beam.h"

Beam::Beam(const sf::Vector2f &pos, sf::Texture &tex, const int &dmg, const sf::Vector2f &dir, const float &cut, const float &dur, const float &width)
    : Attack(pos, tex, 0, 10, dmg, dir, LOGICAL_WIDTH), chargeUpTime(cut), duration(dur), width(width)
{
}

Beam::Beam(const Beam &other)
    : Attack(other), chargeUpTime(other.chargeUpTime), duration(other.duration), width(other.width)
{
}

Beam &Beam::operator=(const Beam &other)
{
    if (this != &other)
    {
        Attack::operator=(other);
        chargeUpTime = other.chargeUpTime;
        duration = other.duration;
        width = other.width;
    }
    return *this;
}

void Beam::doLoad()
{
    sf::Angle angle = sf::degrees(0);

    if (direction != sf::Vector2f(0, 0))
        angle += direction.angle();

    sf::Vector2f scaleFactor = {range, width};
    sf::Vector2f spriteOriginFactor = {0.f, 0.5f};
    sf::Vector2f collisionBoxSizeFactor = {0.f, 0.f}; // relative to sprite dimensions
    sf::Vector2f collisionBoxOriginFactor = {0.f, 0.f}; // relative to collision box
    sf::Vector2f collisionBoxPositionFactor = {0.f, 0.f}; // relative to sprite
    int hitBoxPointCount = 4;
    std::vector<sf::Vector2f> hitBoxPointFactors = { // relative to sprite dimensions (0,0) is top-left, (1,1) is bottom-right
        {0.f, 0.f},
        {1.f, 0.f},
        {1.f, 1.f},
        {0.f, 1.f}
    };
    ObjectLoadParams params(scaleFactor, spriteOriginFactor, collisionBoxSizeFactor, collisionBoxOriginFactor, collisionBoxPositionFactor);

    Entity::load(params, hitBoxPointCount, hitBoxPointFactors);

    transform({0.f, 0.f}, false, angle);

    chargeUp.setSize({range, width / 6.f});
    chargeUp.setOrigin({0.f, chargeUp.getSize().y / 2.f});
    chargeUp.setFillColor(sf::Color::Red);
    chargeUp.setPosition(origin);
    chargeUp.setRotation(angle);
}

bool Beam::doUpdate(const float &dt)
{
    if (chargeUpTime > 0.f)
    {
        chargeUpTime -= dt;
        return false;
    }

    else
    {
        duration -= dt;

        if (duration <= 0.f)
            return true;
    }

    return false;
}

void Beam::doDraw(sf::RenderWindow &window) const
{
    if (chargeUpTime > 0.f)
        window.draw(chargeUp);
    else
        Entity::doDraw(window);
}

int Beam::doHits(const Entity &other) const
{
    if (chargeUpTime > 0.f)
        return 0;
    else
        return Attack::doHits(other);
}

std::ostream &operator<<(std::ostream &os, const Beam &beam)
{
    os << static_cast<const Attack &>(beam);
    os << "\n";
    os << "        Charge up time: " << beam.chargeUpTime << "\n";
    os << "        Lifetime: " << beam.duration << "\n";
    return os;
}
