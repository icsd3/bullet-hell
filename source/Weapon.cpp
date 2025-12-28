#include "../headers/Weapon.h"

Weapon::Weapon(const std::string &nm, const int dmg, const float as, const float off, const sf::Texture &tex)
    : name(nm), texture(&tex), sprite(*texture), damage(dmg), attackSpeed(as)
{
    std::uniform_real_distribution<float> dist(0, off);
    std::mt19937 &rng = Utils::getRng();
    attackSpeed = as / (1 + as * dist(rng));
}

Weapon::Weapon(const Weapon &other)
    : name(other.name), texture(other.texture), sprite(other.sprite), damage(other.damage), attackSpeed(other.attackSpeed), weaponClock(other.weaponClock)
{
}

Weapon &Weapon::operator=(const Weapon &other)
{
    if (this != &other)
    {
        name = other.name;
        damage = other.damage;
        attackSpeed = other.attackSpeed;
        weaponClock = other.weaponClock;
        texture = other.texture;
        sprite = other.sprite;
    }
    return *this;
}

void Weapon::printDetails(std::ostream &os) const
{
    os << "Weapon (Name: " << name
       << ", Damage: " << damage
       << ", Attack speed:" << attackSpeed << ")";
}

std::vector<Projectile> Weapon::fire(const sf::Vector2f &position, const sf::Vector2f &target)
{
    sf::Vector2f origin = position + (target - position).normalized() * 60.f; 
    return doFire(origin, target);
}

std::ostream &operator<<(std::ostream &os, const Weapon &weapon)
{
    weapon.printDetails(os);
    return os;
}

void Weapon::load(sf::Vector2f &position)
{
    float scale = 50.f / static_cast<float>(texture->getSize().x);
    sprite.setScale({scale, scale});
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(sf::Vector2f(5.f, bounds.size.y - 5.f));
    sprite.setPosition(position);
}

void Weapon::reset()
{
    weaponClock.restart();
}

void Weapon::update(sf::Vector2f &pos, const sf::Angle &angle)
{
    if (Utils::changePaused(0))
        weaponClock.stop();
    else
        weaponClock.start();

    sf::Angle rotation = angle + sf::degrees(45);
    rotation = rotation % sf::degrees(360);

    if (rotation.asDegrees() > 135 && rotation.asDegrees() < 315)
    {
        sprite.setScale({sprite.getScale().x, -std::abs(sprite.getScale().y)});
        sprite.setRotation(rotation - sf::degrees(90));
    }
    else
    {
        sprite.setScale({sprite.getScale().x, std::abs(sprite.getScale().y)});
        sprite.setRotation(rotation);
    }

    sprite.setPosition(pos);
}

void Weapon::draw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);
    sf::Sprite drawSprite = sprite;
    drawSprite.setPosition(Utils::mapToScreen(sprite.getPosition(), window));
    drawSprite.scale(scaleFactor);
    window.draw(drawSprite);
}