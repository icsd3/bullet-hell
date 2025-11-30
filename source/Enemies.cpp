#include "../headers/Enemies.h"
#include "../headers/Utils.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

Enemy::Enemy(const bool &ec, const sf::Vector2f &pos, const bool &ori, const std::string &tf, const sf::Texture &tex, const float spd, const int &mh, const Weapon &ew)
    : Entity(ec, pos, ori, tf, tex, spd, mh), weapon(ew)
{
}

Enemy Enemy::spawnEnemy(const sf::Texture &tex, const sf::Vector2f &pos, float spd, const int &mh)
{
    std::ifstream file("json/Weapons.json");
    nlohmann::json data;
    file >> data;
    const auto &w = data[0];
    Weapon ew = Weapon(
        w["name"],
        w["damage"],
        w["bullet_nr"],
        w["fire_rate"],
        w["spread_angle"],
        w["range"],
        w["bullet_speed"],
        0.3f
    );
    Enemy enemy(true, pos, false, "textures/enemy.png", tex, spd, mh, ew);
    return enemy;
}

void Enemy::load()
{
    Entity::load();

    maxHealthBar.setSize(sf::Vector2f(sprite.getGlobalBounds().size.x, LOGICAL_HEIGHT / 100.f));
    maxHealthBar.setFillColor(sf::Color(75, 0, 0, 175));
    maxHealthBar.setOrigin(sf::Vector2f(maxHealthBar.getLocalBounds().size.x / 2.f, maxHealthBar.getLocalBounds().size.y / 2.f));
    maxHealthBar.setPosition(sf::Vector2f(position.x, position.y - sprite.getGlobalBounds().size.y / 2.f - maxHealthBar.getSize().y));
    maxHealthBar.setOutlineThickness(2.f);
    maxHealthBar.setOutlineColor(sf::Color::Black);

    currentHealthBar.setSize(maxHealthBar.getSize());
    currentHealthBar.setFillColor(sf::Color(150, 0, 0, 175));
    currentHealthBar.setOrigin(sf::Vector2f(0, currentHealthBar.getLocalBounds().size.y / 2.f));
    currentHealthBar.setPosition(sf::Vector2f(maxHealthBar.getPosition().x - maxHealthBar.getSize().x / 2.f, maxHealthBar.getPosition().y));

    weapon.reset();
}

void Enemy::draw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    Entity::draw(window);

    sf::RectangleShape drawMaxHealthBar = maxHealthBar;
    drawMaxHealthBar.setPosition(Utils::mapToScreen(maxHealthBar.getPosition(), window));
    drawMaxHealthBar.setSize(sf::Vector2f(maxHealthBar.getSize().x * scaleFactor.x, maxHealthBar.getSize().y * scaleFactor.y));
    drawMaxHealthBar.setOrigin(sf::Vector2f(drawMaxHealthBar.getSize().x / 2.f, drawMaxHealthBar.getSize().y / 2.f));
    
    drawMaxHealthBar.setOutlineThickness(maxHealthBar.getOutlineThickness() * scaleFactor.x);
    
    window.draw(drawMaxHealthBar);

    sf::RectangleShape drawCurrentHealthBar = currentHealthBar;
    drawCurrentHealthBar.setPosition(Utils::mapToScreen(currentHealthBar.getPosition(), window));
    drawCurrentHealthBar.setSize(sf::Vector2f(currentHealthBar.getSize().x * scaleFactor.x, currentHealthBar.getSize().y * scaleFactor.y));
    drawCurrentHealthBar.setOrigin(sf::Vector2f(0, drawCurrentHealthBar.getSize().y / 2.f));
    
    window.draw(drawCurrentHealthBar);
}

std::vector<Projectile> Enemy::update(const sf::Vector2f &target, const sf::Texture &tex)
{
    weapon.update();
    std::vector<Projectile> bullets;
    if (canFire())
    {
        bullets = weapon.fire(position, target, tex);
        weapon.reset();  // Reset cooldown only after actually firing
    }
    return bullets;
}

bool Enemy::canFire()
{
    return weapon.canFire();
}

bool Enemy::takeDamage(const int &dmg)
{
    currentHealth -= dmg;
    currentHealthBar.setSize(sf::Vector2f(1.f * currentHealth / maxHealth * maxHealthBar.getSize().x, currentHealthBar.getSize().y));
    if (currentHealth <= 0)
        return true;
    return false;
}

std::ostream &operator<<(std::ostream &os, const Enemy &enemy)
{
    os << static_cast<const Entity &>(enemy);
    os << "\n";
    os << "        Max Health: " << enemy.maxHealth << "\n";
    os << "        Current Health: " << enemy.currentHealth << "\n";
    os << "        Enemy Weapon: " << enemy.weapon;
    return os;
}
