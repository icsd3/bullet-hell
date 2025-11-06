#include "../headers/Enemies.h"
#include <iostream>

Enemy::Enemy(const bool& ec, const sf::Vector2f& pos, const bool& ori, const std::string& tf, float spd, const int& mh, const Weapon& ew)
    :Entity(ec, pos, ori, tf, spd), maxHealth(mh), enemyWeapon(ew), currentHealth(mh)
{

}

Enemy Enemy::spawnEnemy(const sf::Vector2f& pos, float spd, const int& mh, const Weapon& ew)
{
    Enemy enemy(true, pos, false, "textures/enemy.png", spd, mh, ew);
    return enemy;
}

void Enemy::loadEnemy(sf::RenderWindow& window)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    sprite.setPosition(position);
    sprite.setScale(sf::Vector2f(window.getSize().x / texture.getSize().x / 20.f, window.getSize().x / texture.getSize().x / 20.f));
}

void Enemy::drawEnemy(sf::RenderWindow& window)
{
    window.draw(sprite);
}

bool Enemy::updateEnemy(const int& dmg, const float& dt, const sf::Vector2f& target)
{
    if(takeDamage(dmg))
        return true;

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
    return false;
}

bool Enemy::takeDamage(const int& dmg)
{
    currentHealth -= dmg;
    if(currentHealth <= 0)
        return true;
    return false;
}

Projectile Enemy::fireEnemyWeapon(const sf::Vector2f& target)
{
    Projectile projectile = enemyWeapon.fire(position, target);
    return projectile;
}

bool Enemy::canFireEnemyWeapon(const float& dt)
{
    return enemyWeapon.canFire(dt);
}

Enemy::Enemy(const Enemy& other)
    :Entity(other), maxHealth(other.maxHealth), enemyWeapon(other.enemyWeapon), currentHealth(other.currentHealth)
{

}

Enemy& Enemy::operator=(const Enemy& other)
{
    if (this != &other)
    {
        Entity::operator=(other);
        maxHealth = other.maxHealth;
        enemyWeapon = other.enemyWeapon;
        currentHealth = other.currentHealth;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Enemy& enemy)
{
    os << static_cast<const Entity&>(enemy);
    os << " Max Health: " << enemy.maxHealth;
    os << " Current Health: " << enemy.currentHealth;
    os << " Enemy Weapon: " << enemy.enemyWeapon;
    return os;
}
