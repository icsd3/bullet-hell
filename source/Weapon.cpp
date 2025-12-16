#include "../headers/Weapon.h"

Weapon::Weapon(const std::string &nm, const int dmg, const float as, const float off)
    : name(nm), damage(dmg), attackSpeed(as)
{
    std::uniform_real_distribution<float> dist(0, off);
    std::mt19937 &rng = Utils::getRng();
    attackSpeed = as / (1 + as * dist(rng));
}

Weapon::Weapon(const Weapon &other)
    : name(other.name), damage(other.damage), attackSpeed(other.attackSpeed), weaponClock(other.weaponClock)
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
    }
    return *this;
}

void Weapon::printDetails(std::ostream &os) const
{
    os << "Weapon (Name: " << name
       << ", Damage: " << damage
       << ", Attack speed:" << attackSpeed << ")";
}

std::ostream &operator<<(std::ostream &os, const Weapon &weapon)
{
    weapon.printDetails(os);
    return os;
}

void Weapon::reset()
{
    weaponClock.restart();
}

void Weapon::update()
{
    if (Utils::changePaused(0))
        weaponClock.stop();
    else
        weaponClock.start();
}