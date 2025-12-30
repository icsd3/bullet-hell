#include "../headers/Attack.h"

Attack::Attack(const sf::Vector2f &pos, sf::Texture &tex, float spd, const int &dmg, const sf::Vector2f &dir, const float &rn)
    : Entity(pos, tex, spd, 1), damage(dmg), direction(dir), origin(position), range(rn)
{
}

void Attack::load()
{
    doLoad();
}

bool Attack::update(const float &dt)
{
    return doUpdate(dt);
}

int Attack::doHits(const Entity &other) const
{
    if (Entity::doHits(other))
        return damage;

    return 0;
}

std::ostream &operator<<(std::ostream &os, const Attack &attack)
{
    os << static_cast<const Entity &>(attack);
    os << "\n";
    os << "        Damage: " << attack.damage << "\n";
    os << "        Direction: (" << attack.direction.x << ", " << attack.direction.y << ")\n";
    return os;
}