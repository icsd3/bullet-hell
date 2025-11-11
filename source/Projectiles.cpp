#include "../headers/Projectiles.h"

Projectile::Projectile(const bool &ec, const sf::Vector2f &pos, const bool &ori, const std::string &textureFile, const sf::Texture &tex, float spd, const int &dmg, const sf::Vector2f &t)
    : Entity(ec, pos, ori, textureFile, tex, spd), damage(dmg), target(t)
{
    direction = target - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= distance;
    rotation = sf::radians(std::atan2(direction.y, direction.x));
}

void Projectile::loadProjectile(sf::RenderWindow &window, const sf::Texture &texture)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    sprite.setPosition(position);
    sprite.setScale(sf::Vector2f(1.f * window.getSize().x / texture.getSize().x / 50.f, 1.f * window.getSize().x / texture.getSize().x / 50.f));
    sprite.setRotation(rotation);
}

void Projectile::drawProjectile(sf::RenderWindow &window)
{
    window.draw(sprite);
}

bool Projectile::updateProjectile(const float &dt, sf::RenderWindow &window)
{
    sprite.move(sf::Vector2f(direction * speed * dt));
    position = sprite.getPosition();
    if (position.x < 0 || position.x > window.getSize().x || position.y < 0 || position.y > window.getSize().y)
    {
        return true;
    }
    return false;
}

int Projectile::getDamage() const
{
    return damage;
}

std::ostream &operator<<(std::ostream &os, const Projectile &projectile)
{
    os << static_cast<const Entity &>(projectile);
    os << " Damage: " << projectile.damage;
    os << " Target: (" << projectile.target.x << ", " << projectile.target.y << ")";
    os << " Direction: (" << projectile.direction.x << ", " << projectile.direction.y << ")";
    os << " Rotation: " << projectile.rotation.asDegrees();
    return os;
}