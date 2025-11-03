#include "../headers/Weapons.h"

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
    os << "Weapon (Name: " << weapon.name 
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