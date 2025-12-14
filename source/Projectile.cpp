#include "../headers/Projectile.h"

Projectile::Projectile(const sf::Vector2f &pos, sf::Texture &tex, float spd, const int &dmg, const sf::Vector2f &dir, const float &rn)
    : Entity(pos, tex, spd, 1), damage(dmg), direction(dir), origin(position), range(rn)
{
    rotation = sf::radians(std::atan2(direction.y, direction.x));
}

void Projectile::load()
{   
    collisionBox.setRotation(rotation);
    sprite.setRotation(rotation);

    Entity::load(35.f, {0.5f, 0.5f}, {0.5f, 0.5f}, {0.f, 0.f}, 8, {
        {3.5f / 20, 0.f},
        {16.5f / 20, 0.f},
        {1.f, 3.5f / 10},
        {1.f, 6.5f / 10},
        {16.5f / 20, 1.f},
        {3.5f / 20, 1.f},
        {0.f, 6.5f / 10},
        {0.f, 3.5f / 10}
    });
}

bool Projectile::update(const float &dt)
{
    sprite.move(sf::Vector2f(direction * speed * dt));
    collisionBox.move(sf::Vector2f(direction * speed * dt));
    hitBox.move(sf::Vector2f(direction * speed * dt));
    position = sprite.getPosition();
    sf::Vector2f dir = position - origin;
    float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    
    if (distance > range)
    {
        return true;
    }
    return false;
}

int Projectile::doHits(const Entity &other) const
{
    if (Entity::doHits(other))
        return damage;
    return 0;
}

std::ostream &operator<<(std::ostream &os, const Projectile &projectile)
{
    os << static_cast<const Entity &>(projectile);
    os << "\n";
    os << "        Damage: " << projectile.damage << "\n";
    os << "        Direction: (" << projectile.direction.x << ", " << projectile.direction.y << ")\n";
    os << "        Rotation: " << projectile.rotation.asDegrees();
    return os;
}