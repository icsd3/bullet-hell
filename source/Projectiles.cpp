#include "../headers/Projectiles.h"
#include "../headers/Utils.h"

Projectile::Projectile(const sf::Vector2f &pos, const bool &ori, sf::Texture &tex, float spd, const int &dmg, const sf::Vector2f &dir, const float &rn)
    : Entity(pos, ori, tex, spd, 1), damage(dmg), direction(dir), origin(position), range(rn)
{
    rotation = sf::radians(std::atan2(direction.y, direction.x));
}

void Projectile::load()
{   
    Object::load();

    float scale = 1.f * LOGICAL_WIDTH / static_cast<float>(texture.value()->getSize().x) / 50.f;
    sprite.value().setScale(sf::Vector2f(scale, scale));
    collisionBox.setSize(sf::Vector2f(scale * texture.value()->getSize().x, scale * texture.value()->getSize().y));
    collisionBox.setOrigin(sf::Vector2f(collisionBox.getSize().x / 2.f, collisionBox.getSize().y / 2.f));
    collisionBox.setPosition(sf::Vector2f(position.x, position.y));
    collisionBox.setRotation(rotation);
    sprite.value().setRotation(rotation);

    sf::FloatRect bounds = sprite.value().getLocalBounds();
    sf::Transform t = sprite.value().getTransform();
    hitBox.setFillColor(sf::Color(0, 200, 0, 150));
    hitBox.setPointCount(8);
    hitBox.setPoint(0, t.transformPoint({bounds.position.x + 3.5f / 20 * bounds.size.x, bounds.position.y}));
    hitBox.setPoint(1, t.transformPoint({bounds.position.x + 16.5f / 20 * bounds.size.x, bounds.position.y}));
    hitBox.setPoint(2, t.transformPoint({bounds.position.x + bounds.size.x, bounds.position.y + 3.5f / 10 * bounds.size.y}));
    hitBox.setPoint(3, t.transformPoint({bounds.position.x + bounds.size.x, bounds.position.y + 6.5f / 10 * bounds.size.y}));
    hitBox.setPoint(4, t.transformPoint({bounds.position.x + 16.5f / 20 * bounds.size.x, bounds.position.y + bounds.size.y}));
    hitBox.setPoint(5, t.transformPoint({bounds.position.x + 3.5f / 20 * bounds.size.x, bounds.position.y + bounds.size.y}));
    hitBox.setPoint(6, t.transformPoint({bounds.position.x, bounds.position.y + 6.5f / 10 * bounds.size.y}));
    hitBox.setPoint(7, t.transformPoint({bounds.position.x, bounds.position.y + 3.5f / 10 * bounds.size.y}));
    
    range = LOGICAL_WIDTH * range;
}

bool Projectile::update(const float &dt)
{
    sprite.value().move(sf::Vector2f(direction * speed * LOGICAL_WIDTH * dt));
    collisionBox.move(sf::Vector2f(direction * speed * LOGICAL_WIDTH * dt));
    hitBox.move(sf::Vector2f(direction * speed * LOGICAL_WIDTH * dt));
    position = sprite.value().getPosition();
    sf::Vector2f dir = position - origin;
    float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    
    if (position.x < 100 || position.x > LOGICAL_WIDTH - 100 || position.y < 100 || position.y > LOGICAL_HEIGHT - 100 || distance > range)
    {
        return true;
    }
    return false;
}

int Projectile::hits(const Entity &other) const
{
    if (Entity::hits(other))
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