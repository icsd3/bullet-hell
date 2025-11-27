#include "../headers/Projectiles.h"
#include "../headers/Utils.h"

Projectile::Projectile(const bool &ec, const sf::Vector2f &pos, const bool &ori, const std::string &textureFile, const sf::Texture &tex, float spd, const int &dmg, const sf::Vector2f &dir, const float &rn)
    : Entity(ec, pos, ori, textureFile, tex, spd), damage(dmg), direction(dir), origin(position), range(rn)
{
    rotation = sf::radians(std::atan2(direction.y, direction.x));
}

void Projectile::load(const sf::Texture &texture)
{   
    speed = speed * LOGICAL_WIDTH;
    
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    sprite.setPosition(position);
    
    float scale = 1.f * LOGICAL_WIDTH / static_cast<float>(texture.getSize().x) / 50.f;
    sprite.setScale(sf::Vector2f(scale, scale));
    
    sprite.setRotation(rotation);
    range = LOGICAL_WIDTH * range;
}

void Projectile::draw(sf::RenderWindow &window)
{
    sf::Sprite drawSprite = sprite;
    drawSprite.setPosition(mapToScreen(position, window));
    sf::Vector2f scaleFactor = getScaleFactor(window);
    drawSprite.scale(scaleFactor);
    window.draw(drawSprite);
}

bool Projectile::update(const float &dt)
{
    sprite.move(sf::Vector2f(direction * speed * dt));
    position = sprite.getPosition();
    sf::Vector2f dir = position - origin;
    float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    
    if (position.x < 100 || position.x > LOGICAL_WIDTH - 100 || position.y < 100 || position.y > LOGICAL_HEIGHT - 100 || distance > range)
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
    os << "\n";
    os << "        Damage: " << projectile.damage << "\n";
    os << "        Direction: (" << projectile.direction.x << ", " << projectile.direction.y << ")\n";
    os << "        Rotation: " << projectile.rotation.asDegrees();
    return os;
}