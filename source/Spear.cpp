#include "../headers/Spear.h"

Spear::Spear(const std::string &file_path, const size_t index, AttackTextureType type) 
    : Weapon("", 0, 0, 0, ResourceManager::getTexture(TextureType::Spear), type, 0), duration(0), stabSpeed(0)
{
    std::ifstream file(file_path);
    if (!file.is_open())
        throw ConfigurationException("Failed to open " + file_path);

    nlohmann::json data;
    try 
    {
        file >> data;

        if (index >= data.size())
            throw ConfigurationException("Invalid index " + std::to_string(index) + " for " + file_path);

        const auto &w = data[index];

        name = w.at("name").get<std::string>();
        damage = w.at("damage").get<int>();
        attackSpeed = w.at("attack_speed").get<float>();
        duration = w.at("duration").get<float>();
        stabSpeed = w.at("stab_speed").get<float>();
        range = w.at("range").get<float>();
    }
    catch (const nlohmann::json::parse_error& e) 
    {
        throw ConfigurationException("Failed to parse " + file_path + ": " + std::string(e.what()));
    }
    catch (const nlohmann::json::exception& e) 
    {
        throw ConfigurationException(file_path + " is missing required fields or has invalid types: " + std::string(e.what()));
    }
}

Spear::Spear(const Spear &other) 
    : Weapon(other), duration(other.duration), stabSpeed(other.stabSpeed)
{
}

Spear &Spear::operator=(const Spear &other)
{
    if (this != &other)
    {
        Weapon::operator=(other);
        duration = other.duration;
        stabSpeed = other.stabSpeed;
    }
    return *this;
}

void Spear::printDetails(std::ostream &os) const
{
    Weapon::printDetails(os);
    os << ", Spear: (Duration: " << duration << ")\n";
}

std::vector<std::unique_ptr<Attack>> Spear::doAttack(const sf::Vector2f &position, const sf::Vector2f &target)
{
    std::vector<std::unique_ptr<Attack>> attacks;

    if (weaponClock.getElapsedTime().asSeconds() >= (1 / attackSpeed))
    {
        weaponClock.restart();

        sf::Texture *stabTexture = nullptr;

        if (attackTextureID == AttackTextureType::Player)
            stabTexture = &ResourceManager::getTexture(TextureType::PlayerStab);

        else if (attackTextureID == AttackTextureType::Enemy)
            stabTexture = &ResourceManager::getTexture(TextureType::EnemyStab);

        else if (attackTextureID == AttackTextureType::Boss)
            stabTexture = &ResourceManager::getTexture(TextureType::BossStab);

        else 
            throw ConfigurationException("Invalid AttackTextureType in Spear::doAttack");

        sf::Vector2f direction = target - position;
        if (direction != sf::Vector2f(0, 0))
            direction = direction.normalized();

        float attackOriginOffset = 50.f;

        sf::Vector2f origin = position + direction * attackOriginOffset;

        attacks.push_back(std::make_unique<Stab>(origin, *stabTexture, damage, direction, range, duration, stabSpeed));
    }
    
    return attacks;
}
