#include "../headers/Attack.h"

Attack::Attack(const sf::Vector2f &pos, sf::Texture &tex, float spd, const int &pierce, const int &dmg, const sf::Vector2f &dir, const float &rn)
    : Entity(pos, tex, spd, pierce), damage(dmg), direction(dir), origin(position), range(rn)
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
    if (hitList.contains(&other))
        return 0;

    if (Entity::doHits(other))
    {
        hitList.insert(&other);
        return damage;
    }

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