#include "../headers/Gun.h"

Gun::Gun(const std::string &nm, const int dmg, const float as, const float off, const int bnr, const float sa, const float rg, const float bs, sf::Texture &tex)
    : Weapon(nm, dmg, as, off), bullet_nr(bnr), spread_angle(sa), range(rg), bulletSpeed(bs), projectileTexture(tex)
{
}

Gun::Gun(const Gun &other)
    : Weapon(other), bullet_nr(other.bullet_nr), spread_angle(other.spread_angle), range(other.range),
      bulletSpeed(other.bulletSpeed), projectileTexture(other.projectileTexture)
{
}

Gun &Gun::operator=(const Gun &other)
{
    if (this != &other)
    {
        Weapon::operator=(other);
        bullet_nr = other.bullet_nr;
        spread_angle = other.spread_angle;
        range = other.range;
        bulletSpeed = other.bulletSpeed;
        projectileTexture = other.projectileTexture;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Gun &gun)
{
    os << static_cast<const Weapon &>(gun)
       << ", Bullet Number: " << gun.bullet_nr
       << ", Spread Angle: " << gun.spread_angle
       << ", Range: " << gun.range
       << ", Bullet Speed: " << gun.bulletSpeed << ")";
    return os;
}

std::vector<Projectile> Gun::fire(const sf::Vector2f &position, const sf::Vector2f &target)
{
    std::vector<Projectile> bullets;
    if (weaponClock.getElapsedTime().asSeconds() >= (1 / attackSpeed))
    {
        weaponClock.restart();
        for (int i = 0; i < bullet_nr; i++)
        {
            sf::Vector2f direction = target - position;
            direction = direction.normalized();
            std::uniform_real_distribution<float> dist(spread_angle * -0.5f, spread_angle * 0.5f);
            std::mt19937 &rng = Utils::getRng();
            sf::Angle randomAngle = sf::degrees(dist(rng));
            direction = direction.rotatedBy(randomAngle);
            Projectile projectile = Projectile(position, projectileTexture, bulletSpeed, damage, direction, range);
            bullets.push_back(projectile);
        }
    }
    return bullets;
}