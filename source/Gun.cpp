#include "../headers/Gun.h"

Gun::Gun(const std::string &file_path, const size_t index, const float offset, const AttackTextureType id)
    : Weapon("", 0, 0, offset, ResourceManager::getTexture(TextureType::Pistol), id, 0), bullet_nr(0), spreadAngle(0), bulletSpeed(0)
{
    std::ifstream file(file_path);
    if (!file.is_open())
        throw ConfigurationException("Failed to open " + file_path);

    nlohmann::json data;
    try 
    {
        file >> data;
    } 
    catch (const nlohmann::json::parse_error& e) 
    {
        throw ConfigurationException("Failed to parse " + file_path + ": " + std::string(e.what()));
    }

    if (index >= data.size())
        throw ConfigurationException("Invalid index " + std::to_string(index) + " for " + file_path);

    const auto &w = data[index];
    try 
    {
        int texId = w.at("texture").get<int>();
        switch (texId)
        {
            case 0:
                texture = &ResourceManager::getTexture(TextureType::Pistol);
                break;
            case 1:
                texture = &ResourceManager::getTexture(TextureType::Shotgun);
                break;
            case 2:
                texture = &ResourceManager::getTexture(TextureType::Rifle);
                break;
            case 3:
                texture = &ResourceManager::getTexture(TextureType::Sniper);
                break;
            default:
                throw ConfigurationException(file_path + " is missing required fields or has invalid types");
                break;
        }

        name = w.at("name").get<std::string>();
        damage = w.at("damage").get<int>();
        float as = w.at("attack_speed").get<float>();
        
        std::uniform_real_distribution<float> dist(0, offset);
        std::mt19937 &rng = Utils::getRng();
        attackSpeed = as / (1 + as * dist(rng));

        bullet_nr = w.at("bullet_nr").get<int>();
        spreadAngle = w.at("spread_angle").get<float>();
        range = w.at("range").get<float>();
        bulletSpeed = w.at("bullet_speed").get<float>();
        pierce = w.at("pierce").get<int>();
        
        sprite.setTexture(*texture);
    } 
    catch (const nlohmann::json::exception& e) 
    {
        throw ConfigurationException(file_path + " is missing required fields or has invalid types: " + std::string(e.what()));
    }
}

Gun::Gun(const Gun &other)
    : Weapon(other), bullet_nr(other.bullet_nr), spreadAngle(other.spreadAngle), bulletSpeed(other.bulletSpeed)
{
}

Gun &Gun::operator=(const Gun &other)
{
    if (this != &other)
    {
        Weapon::operator=(other);
        bullet_nr = other.bullet_nr;
        spreadAngle = other.spreadAngle;
        bulletSpeed = other.bulletSpeed;
    }
    return *this;
}

std::vector<std::unique_ptr<Attack>> Gun::doAttack(const sf::Vector2f &position, const sf::Vector2f &target)
{
    std::vector<std::unique_ptr<Attack>> attacks;
    if (weaponClock.getElapsedTime().asSeconds() >= (1 / attackSpeed))
    {
        weaponClock.restart();
        sf::Texture *projectileTexture = nullptr;

        if (attackTextureID == AttackTextureType::Player)
            projectileTexture = &ResourceManager::getTexture(TextureType::PlayerProjectile);

        else if (attackTextureID == AttackTextureType::Enemy)
            projectileTexture = &ResourceManager::getTexture(TextureType::EnemyProjectile);

        else if (attackTextureID == AttackTextureType::Boss)
            projectileTexture = &ResourceManager::getTexture(TextureType::BossProjectile);

        else 
            throw ConfigurationException("Invalid AttackTextureType in Gun::doAttack");

        for (int i = 0; i < bullet_nr; i++)
        {
            sf::Vector2f direction = target - position;
            direction = direction.normalized();

            sf::Vector2f origin = position + direction * 75.f; 

            std::uniform_real_distribution<float> dist(spreadAngle * -0.5f, spreadAngle * 0.5f);
            std::mt19937 &rng = Utils::getRng();
            sf::Angle randomAngle = sf::degrees(dist(rng));
            direction = direction.rotatedBy(randomAngle);

            attacks.push_back(std::make_unique<Projectile>(origin, *projectileTexture, bulletSpeed, pierce, damage, direction, range - 75.f));
        }
    }
    return attacks;
}

void Gun::printDetails(std::ostream &os) const
{
    Weapon::printDetails(os);
    os << ", Gun: (Bullet Number: " << bullet_nr
       << ", Spread Angle: " << spreadAngle
       << ", Range: " << range
       << ", Bullet Speed: " << bulletSpeed << ")";
}