#include "../headers/Gun.h"

Gun::Gun(const std::string &nm, const int dmg, const float as, const float off, const int bnr, const float sa, const float rg, const float bs, const ProjectileTextureType id, const sf::Texture &tex)
    : Weapon(nm, dmg, as, off, tex), bullet_nr(bnr), spread_angle(sa), range(rg), bulletSpeed(bs), projectileTextureID(id)
{
}

Gun::Gun(const Gun &other)
    : Weapon(other), bullet_nr(other.bullet_nr), spread_angle(other.spread_angle), range(other.range),
      bulletSpeed(other.bulletSpeed), projectileTextureID(other.projectileTextureID)
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
        projectileTextureID = other.projectileTextureID;
    }
    return *this;
}

std::vector<std::unique_ptr<Attack>> Gun::doFire(const sf::Vector2f &position, const sf::Vector2f &target)
{
    std::vector<std::unique_ptr<Attack>> bullets;
    if (weaponClock.getElapsedTime().asSeconds() >= (1 / attackSpeed))
    {
        weaponClock.restart();
        sf::Texture *projectileTexture = nullptr;

        if (projectileTextureID == ProjectileTextureType::Player)
            projectileTexture = &ResourceManager::getTexture(TextureType::PlayerProjectile);

        else if (projectileTextureID == ProjectileTextureType::Enemy)
            projectileTexture = &ResourceManager::getTexture(TextureType::EnemyProjectile);

        else if (projectileTextureID == ProjectileTextureType::Boss)
            projectileTexture = &ResourceManager::getTexture(TextureType::BossProjectile);

        else 
            throw ConfigurationException("Invalid ProjectileTextureType in Gun::doFire");

        for (int i = 0; i < bullet_nr; i++)
        {
            sf::Vector2f direction = target - position;
            direction = direction.normalized();

            std::uniform_real_distribution<float> dist(spread_angle * -0.5f, spread_angle * 0.5f);
            std::mt19937 &rng = Utils::getRng();
            sf::Angle randomAngle = sf::degrees(dist(rng));
            direction = direction.rotatedBy(randomAngle);

            bullets.push_back(std::make_unique<Projectile>(position, *projectileTexture, bulletSpeed, damage, direction, range));
        }
    }
    return bullets;
}

void Gun::printDetails(std::ostream &os) const
{
    Weapon::printDetails(os);
    os << ", Gun: (Bullet Number: " << bullet_nr
       << ", Spread Angle: " << spread_angle
       << ", Range: " << range
       << ", Bullet Speed: " << bulletSpeed << ")";
}