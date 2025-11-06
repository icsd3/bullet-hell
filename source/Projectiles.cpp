#include "../headers/Projectiles.h"

Projectile::Projectile(const bool& ec, const sf::Vector2f& pos, const bool& ori, const std::string& textureFile, float spd, const int& dmg, const sf::Vector2f& t)
    :Entity(ec, pos, ori, textureFile, spd), damage(dmg), target(t)
{

}

void Projectile::loadProjectile(sf::RenderWindow& window)
{
    sf::FloatRect bounds = sprite->getLocalBounds();
    sprite->setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    sprite->setPosition(position);
    sprite->setScale(sf::Vector2f(window.getSize().x / texture.getSize().y / 20.f, window.getSize().x / texture.getSize().y / 20.f));
    sprite->setRotation(rotation);
}