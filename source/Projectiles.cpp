#include "../headers/Projectiles.h"
#include "../headers/Utils.h"

Projectile::Projectile(const bool &ec, const sf::Vector2f &pos, const bool &ori, const std::string &textureFile, const sf::Texture &tex, float spd, const int &dmg, const sf::Vector2f &dir, const float &rn)
    : Entity(ec, pos, ori, textureFile, tex, spd, 1), damage(dmg), direction(dir), origin(position), range(rn)
{
    rotation = sf::radians(std::atan2(direction.y, direction.x));
}

void Projectile::load()
{   
    Object::load();

    float scale = 1.f * LOGICAL_WIDTH / static_cast<float>(texture.getSize().x) / 50.f;
    sprite.setScale(sf::Vector2f(scale, scale));
    collisionBox.setSize(sf::Vector2f(scale * texture.getSize().x, scale * texture.getSize().y));
    collisionBox.setOrigin(sf::Vector2f(collisionBox.getSize().x / 2.f, collisionBox.getSize().y / 2.f));
    collisionBox.setPosition(sf::Vector2f(position.x, position.y));
    collisionBox.setRotation(rotation);
    sprite.setRotation(rotation);

    sf::FloatRect bounds = sprite.getLocalBounds();
    sf::Vector2f topLeft(bounds.position);
    sf::Vector2f topRight(bounds.position.x + bounds.size.x, bounds.position.y);
    sf::Vector2f bottomLeft(bounds.position.x, bounds.position.y + bounds.size.y);
    sf::Vector2f bottomRight(bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y);

    sf::Transform t = sprite.getTransform();

    sf::Vector2f worldTopLeft     = t.transformPoint(topLeft);
    sf::Vector2f worldTopRight    = t.transformPoint(topRight);
    sf::Vector2f worldBottomLeft  = t.transformPoint(bottomLeft);
    sf::Vector2f worldBottomRight = t.transformPoint(bottomRight);

    hitBox.setPointCount(4);
    hitBox.setPoint(0, worldTopLeft);
    hitBox.setPoint(1, worldTopRight);
    hitBox.setPoint(2, worldBottomRight);
    hitBox.setPoint(3, worldBottomLeft);

    hitBox.setFillColor(sf::Color(0, 255, 0, 255));

    range = LOGICAL_WIDTH * range;
}

bool Projectile::update(const float &dt)
{
    sprite.move(sf::Vector2f(direction * speed * LOGICAL_WIDTH * dt));
    collisionBox.move(sf::Vector2f(direction * speed * LOGICAL_WIDTH * dt));
    hitBox.move(sf::Vector2f(direction * speed * LOGICAL_WIDTH * dt));
    position = sprite.getPosition();
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