#include "../headers/Player.h"

Player::Player()
    :Entity(sf::Vector2f(400.f, 300.f), sf::degrees(0), "textures/player.png", 100.f), max_health(100), current_health(100)
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
       << ", Max Health: " << player.max_health 
       << ", Current Health: " << player.current_health 
       << ", Weapons Count: " << player.weapons.size();
    return os;
}

void Player::updatePlayer()
{
    
}

void Player::loadPlayer(sf::RenderWindow& window)
{
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = texture.getSize();
    sf::FloatRect bounds = sprite->getLocalBounds();
    sprite->setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    sprite->setPosition(position);
    sprite->setRotation(angle);
    sprite->setScale(sf::Vector2f(0.1f, 0.1f));
    
    std::cout << "Player loaded at position: " << position.x << ", " << position.y << " origin: " << sprite->getOrigin().x << ", " << sprite->getOrigin().y << "\n";
}

Weapon::Weapon(const std::string& nm, int dmg, int bnr, float fr, float sa, float rg)
    :name(nm), damage(dmg), bullet_nr(bnr), fire_rate(fr), spread_angle(sa), range(rg)
{

}

Weapon::Weapon(const Weapon& other)
    :name(other.name), damage(other.damage), bullet_nr(other.bullet_nr),
     fire_rate(other.fire_rate), spread_angle(other.spread_angle), range(other.range)
{

}

Weapon& Weapon::operator=(const Weapon& other)
{
    if (this != &other)
    {
        damage = other.damage;
        bullet_nr = other.bullet_nr;
        fire_rate = other.fire_rate;
        spread_angle = other.spread_angle;
        range = other.range;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Weapon& weapon)
{
    os << "Weapon(Name: " << weapon.name 
       << ", Damage: " << weapon.damage 
       << ", Bullet Number: " << weapon.bullet_nr
       << ", Fire Rate: " << weapon.fire_rate 
       << ", Spread Angle: " << weapon.spread_angle 
       << ", Range: " << weapon.range << ")";
    return os;
}

Weapon::~Weapon()
{
    std::cout<<"Weapon destroyed: "<<name<<"\n";
}