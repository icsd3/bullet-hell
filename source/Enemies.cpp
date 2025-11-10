#include "../headers/Enemies.h"
#include <iostream>

Enemy::Enemy(const bool &ec, const sf::Vector2f &pos, const bool &ori, const std::string &tf, const sf::Texture &tex, float spd, const int &mh, const Weapon &ew)
    : Entity(ec, pos, ori, tf, tex, spd), maxHealth(mh), enemyWeapon(ew), currentHealth(mh)
{
}

Enemy Enemy::spawnEnemy(const sf::Texture &tex, const sf::Vector2f &pos, float spd, const int &mh, const Weapon &ew)
{
    Enemy enemy(true, pos, false, "textures/enemy.png", tex, spd, mh, ew);
    return enemy;
}

void Enemy::loadEnemy(sf::RenderWindow &window, const sf::Texture &texture)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    sprite.setPosition(position);
    sprite.setScale(sf::Vector2f(1.f * window.getSize().x / texture.getSize().x / 20.f, 1.f * window.getSize().x / texture.getSize().x / 20.f));

    maxHealthBar.setSize(sf::Vector2f(sprite.getGlobalBounds().size.x, window.getSize().y / 100.f));
    maxHealthBar.setFillColor(sf::Color(75, 0, 0, 175));
    maxHealthBar.setOrigin(sf::Vector2f(maxHealthBar.getLocalBounds().size.x / 2.f, maxHealthBar.getLocalBounds().size.y / 2.f));
    maxHealthBar.setPosition(sf::Vector2f(position.x, position.y - sprite.getGlobalBounds().size.y / 2.f - maxHealthBar.getSize().y));
    maxHealthBar.setOutlineThickness(2.f);
    maxHealthBar.setOutlineColor(sf::Color::Black);

    currentHealthBar.setSize(maxHealthBar.getSize());
    currentHealthBar.setFillColor(sf::Color(150, 0, 0, 175));
    currentHealthBar.setOrigin(sf::Vector2f(0, currentHealthBar.getLocalBounds().size.y / 2.f));
    currentHealthBar.setPosition(sf::Vector2f(maxHealthBar.getPosition().x - maxHealthBar.getSize().x / 2.f, maxHealthBar.getPosition().y));
}

void Enemy::drawEnemy(sf::RenderWindow &window)
{
    window.draw(sprite);
    window.draw(maxHealthBar);
    window.draw(currentHealthBar);
}

// void Enemy::updateEnemy(const float &dt, const sf::Vector2f &target)
// {
//     sf::Vector2f dir = target - position;
//     float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

//     if (distance > 5.0f)
//     {
//         dir /= distance;
//         sprite.move(sf::Vector2f(dir * speed * dt));
//         position = sprite.getPosition();
//         if (dir.x > 0.f)
//         {
//             orientation = true;
//             sprite.setScale(sf::Vector2f(-std::abs(sprite.getScale().x), sprite.getScale().y));
//         }
//         else
//         {
//             orientation = false;
//             sprite.setScale(sf::Vector2f(std::abs(sprite.getScale().x), sprite.getScale().y));
//         }
//     }
// }

bool Enemy::takeDamage(const int &dmg)
{
    currentHealth -= dmg;
    currentHealthBar.setSize(sf::Vector2f(static_cast<float>(currentHealth) / static_cast<float>(maxHealth) * maxHealthBar.getSize().x, currentHealthBar.getSize().y));
    if (currentHealth <= 0)
        return true;
    return false;
}

// Projectile Enemy::fireEnemyWeapon(const sf::Vector2f &target, const sf::Texture &tex)
// {
//     Projectile projectile = enemyWeapon.fire(position, target, tex);
//     return projectile;
// }

// bool Enemy::canFireEnemyWeapon(const float &dt)
// {
//     return enemyWeapon.canFire(dt);
// }

Enemy::Enemy(const Enemy &other)
    : Entity(other), maxHealth(other.maxHealth), enemyWeapon(other.enemyWeapon), currentHealth(other.currentHealth), maxHealthBar(other.maxHealthBar), currentHealthBar(other.currentHealthBar)
{
}

Enemy &Enemy::operator=(const Enemy &other)
{
    if (this != &other)
    {
        Entity::operator=(other);
        maxHealth = other.maxHealth;
        enemyWeapon = other.enemyWeapon;
        currentHealth = other.currentHealth;
        maxHealthBar = other.maxHealthBar;
        currentHealthBar = other.currentHealthBar;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Enemy &enemy)
{
    os << static_cast<const Entity &>(enemy);
    os << " Max Health: " << enemy.maxHealth;
    os << " Current Health: " << enemy.currentHealth;
    os << " Enemy Weapon: " << enemy.enemyWeapon;
    return os;
}
