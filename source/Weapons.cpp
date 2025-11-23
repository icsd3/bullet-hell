#include "../headers/Weapons.h"
#include <iostream>

Weapon::Weapon(const std::string &nm, int dmg, int bnr, float fr, float sa, float rg, float bs)
    : name(nm), damage(dmg), bullet_nr(bnr), fire_rate(fr), spread_angle(sa), range(rg), bulletSpeed(bs)
{
}

Weapon::Weapon(const Weapon &other)
    : name(other.name), damage(other.damage), bullet_nr(other.bullet_nr),
      fire_rate(other.fire_rate), spread_angle(other.spread_angle), range(other.range), bulletSpeed(other.bulletSpeed)
{
}

Weapon &Weapon::operator=(const Weapon &other)
{
    if (this != &other)
    {
        damage = other.damage;
        bullet_nr = other.bullet_nr;
        fire_rate = other.fire_rate;
        spread_angle = other.spread_angle;
        range = other.range;
        bulletSpeed = other.bulletSpeed;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Weapon &weapon)
{
    os << "Weapon (Name: " << weapon.name
       << ", Damage: " << weapon.damage
       << ", Bullet Number: " << weapon.bullet_nr
       << ", Fire Rate: " << weapon.fire_rate
       << ", Spread Angle: " << weapon.spread_angle
       << ", Range: " << weapon.range
       << ", Bullet Speed: " << weapon.bulletSpeed << ")";
    return os;
}

std::vector<Projectile> Weapon::fire(const sf::Vector2f &position, const sf::Vector2f &target, const sf::Texture &texture) const
{
    std::vector<Projectile> bullets;
    for(int i = 0; i < bullet_nr; i++)
    {
        sf::Vector2f direction = target - position;
        direction = direction.normalized();
        sf::Angle randomAngle = sf::degrees(((rand() / (float)RAND_MAX) - 0.5f) * spread_angle);
        direction = direction.rotatedBy(randomAngle);
        Projectile projectile = Projectile(true, position, true, "textures/player_projectile.png", texture, bulletSpeed, damage, direction, range);
        bullets.push_back(projectile);
    }
    return bullets;
}

bool Weapon::canFire()
{
    float dt = weaponClock.getElapsedTime().asSeconds();
    if (dt >= 1 / fire_rate)
    {
        weaponClock.restart();
        return true;
    }
    return false;
}

void Weapon::reset()
{
    weaponClock.restart();
}