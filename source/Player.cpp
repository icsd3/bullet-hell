#include "../headers/Player.h"
#include "../headers/Utils.h"

Player::Player(const sf::Vector2f &start, const sf::Texture &tex)
    : Entity(false, start, false, "textures/player.png", tex, 0.1f), maxHealth(100), currentHealth(100), currentWeapon("Basic Gun", 20, 10, 2.f, 45.f, 0.3f, 0.25f)
{                                                                                                                     //name, dmg, b_nr, f_rate, spread, range, spd
    weapons.emplace_back(currentWeapon);
}

Player &Player::getInstance(const sf::Vector2f &start, const sf::Texture &tex)
{
    static Player instance(start, tex);
    return instance;
}

std::ostream &operator<<(std::ostream &os, const Player &player)
{
    os << static_cast<const Entity &>(player);
    os << ", Max Health: " << player.maxHealth;
    os << ", Current Health: " << player.currentHealth;
    os << ", Weapons Count: " << player.weapons.size();
    for (unsigned int i = 0; i < player.weapons.size(); i++)
    {
        os << "\n        Weapon " << i + 1 << ": " << player.weapons[i];
    }
    return os;
}

void Player::update(const float &dt, const sf::Vector2f &target)
{
    sf::Vector2f dir = target - position;
    float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (distance > 5.0f)
    {
        dir /= distance;
        sprite.move(sf::Vector2f(dir * speed * dt));
        position = sprite.getPosition();
        if (dir.x > 0.f)
        {
            orientation = true;
            sprite.setScale(sf::Vector2f(-std::abs(sprite.getScale().x), sprite.getScale().y));
        }
        else
        {
            orientation = false;
            sprite.setScale(sf::Vector2f(std::abs(sprite.getScale().x), sprite.getScale().y));
        }
    }
}

void Player::load(const sf::Texture &texture)
{
    speed = speed * LOGICAL_WIDTH;
    
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    sprite.setPosition(position);

    float scale = 1.f * LOGICAL_WIDTH / texture.getSize().x / 20.f;
    sprite.setScale(sf::Vector2f(scale, scale));
    
    if (orientation)
    {
        sprite.setScale(sf::Vector2f(-std::abs(sprite.getScale().x), sprite.getScale().y));
    }
    else
    {
        sprite.setScale(sf::Vector2f(std::abs(sprite.getScale().x), sprite.getScale().y));
    }
}

void Player::draw(sf::RenderWindow &window)
{
    sf::Sprite drawSprite = sprite;
    drawSprite.setPosition(mapToScreen(position, window));
    sf::Vector2f scaleFactor = getScaleFactor(window);
    drawSprite.scale(scaleFactor);
    window.draw(drawSprite);
}

sf::Vector2i Player::getHealthStatus() const
{
    return sf::Vector2i(currentHealth, maxHealth);
}

sf::Vector2f Player::getPosition() const
{
    return position;
}

std::vector<Projectile> Player::fireCurrentWeapon(const sf::Vector2f &target, const sf::Texture &tex)
{
    return currentWeapon.fire(position, target, tex);
}

bool Player::canFireCurrentWeapon()
{
    return currentWeapon.canFire();
}