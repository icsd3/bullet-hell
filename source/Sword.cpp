#include "../headers/Sword.h"

Sword::Sword(const std::string &file_path, const size_t index, AttackTextureType type)
    : Weapon("", 0, 0, 0, ResourceManager::getTexture(TextureType::Sword), type, 0), duration(0), arcAngle(0) 
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
        name = w.at("name").get<std::string>();
        damage = w.at("damage").get<int>();
        attackSpeed = w.at("attack_speed").get<float>();
        duration = w.at("duration").get<float>();
        arcAngle = w.at("arc_angle").get<float>();
        range = w.at("range").get<float>();
    } 
    catch (const nlohmann::json::exception& e) 
    {
        throw ConfigurationException(file_path + " is missing required fields or has invalid types: " + std::string(e.what()));
    }
}

Sword::Sword(const Sword &other)
    : Weapon(other), duration(other.duration), arcAngle(other.arcAngle)
{
}

Sword &Sword::operator=(const Sword &other)
{
    if (this != &other)
    {
        Weapon::operator=(other);
        duration = other.duration;
        arcAngle = other.arcAngle;
    }
    return *this;
}

void Sword::printDetails(std::ostream &os) const
{
    Weapon::printDetails(os);
    os << ", Sword: (Duration: " << duration 
       << ", Arc Angle: " << arcAngle << ")";
}

std::vector<std::unique_ptr<Attack>> Sword::doAttack(const sf::Vector2f &position, const sf::Vector2f &target)
{
    std::vector<std::unique_ptr<Attack>> attacks;

    if (weaponClock.getElapsedTime().asSeconds() >= (1 / attackSpeed))
    {
        weaponClock.restart();

        sf::Texture *slashTexture = nullptr;

        if (attackTextureID == AttackTextureType::Player)
            slashTexture = &ResourceManager::getTexture(TextureType::PlayerSlash);

        else if (attackTextureID == AttackTextureType::Enemy)
            slashTexture = &ResourceManager::getTexture(TextureType::EnemySlash);

        else if (attackTextureID == AttackTextureType::Boss)
            slashTexture = &ResourceManager::getTexture(TextureType::BossSlash);

        else 
            throw ConfigurationException("Invalid AttackTextureType in Sword::doAttack");

        sf::Vector2f direction = target - position;
        if (direction != sf::Vector2f(0, 0))
            direction = direction.normalized();

        sf::Vector2f origin = position + direction * 50.f; 

        attacks.push_back(std::make_unique<Slash>(origin, *slashTexture, damage, direction, range, duration, arcAngle));
    }
    
    return attacks;
}
