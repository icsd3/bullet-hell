#include "../headers/Player.h"

Player::Player()
    :Entity(false, sf::Vector2f(1000.f, 1000.f), false, "textures/player.png", 175.f), maxHealth(100), currentHealth(100)
{
    weapons.emplace_back("Basic Gun", 10, 1, 0.5f, 0.f, 500.f);
}

Player& Player::getInstance()
{
    static Player instance;
    return instance;
}

std::ostream& operator<<(std::ostream& os, const Player& player)
{
    os << static_cast<const Entity&>(player) 
       << ", Max Health: " << player.maxHealth 
       << ", Current Health: " << player.currentHealth 
       << ", Weapons Count: " << player.weapons.size();
    return os;
}

void Player::updatePlayer(const float& dt, const sf::Vector2f& target)
{
    sf::Vector2f dir = target - position;
    float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (distance > 5.0f) 
    {
        dir /= distance;
        sprite->move(sf::Vector2f(dir * speed * dt));
        position = sprite->getPosition();
        if (dir.x > 0.f)
        {
            orientation = true;
            sprite->setScale(sf::Vector2f(-std::abs(sprite->getScale().x), sprite->getScale().y));
        }
        else
        {
            orientation = false;
            sprite->setScale(sf::Vector2f(std::abs(sprite->getScale().x), sprite->getScale().y));
        }

        // std::cout << "Player updated to position: " << position.x << ", " << position.y << " orientation: " << orientation << "\n";
    }
}

void Player::loadPlayer()
{
    sf::FloatRect bounds = sprite->getLocalBounds();
    sprite->setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    sprite->setPosition(position);
    sprite->setScale(sf::Vector2f(0.1f, 0.1f));
    if(orientation)
    {
        sprite->setScale(sf::Vector2f(-std::abs(sprite->getScale().x), sprite->getScale().y));
    }
    else
    {
        sprite->setScale(sf::Vector2f(std::abs(sprite->getScale().x), sprite->getScale().y));
    }
    
    std::cout << "Player loaded at position: " << position.x << ", " << position.y << " origin: " << sprite->getOrigin().x << ", " << sprite->getOrigin().y << "\n";
}