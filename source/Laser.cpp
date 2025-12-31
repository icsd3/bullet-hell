#include "../headers/Laser.h"

Laser::Laser(const std::string &file_path, const size_t index, const float offset, const AttackTextureType id) 
    : Weapon("", 0, 0, offset, ResourceManager::getTexture(TextureType::Laser), id, 0), chargeUpTime(0.f), duration(0.f), width(0.f)
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
        float as = w.at("attack_speed").get<float>();
        
        std::uniform_real_distribution<float> dist(0, offset);
        std::mt19937 &rng = Utils::getRng();
        attackSpeed = as / (1 + as * dist(rng));

        chargeUpTime = w.at("charge_up_time").get<float>();
        duration = w.at("duration").get<float>();
        width = w.at("width").get<float>();
    } 
    catch (const nlohmann::json::exception& e) 
    {
        throw ConfigurationException(file_path + " is missing required fields or has invalid types: " + std::string(e.what()));
    }
}

Laser::Laser(const Laser &other)
    : Weapon(other), chargeUpTime(other.chargeUpTime), duration(other.duration), width(other.width)
{
}

Laser &Laser::operator=(const Laser &other)
{
    if (this != &other)
    {
        Weapon::operator=(other);
        chargeUpTime = other.chargeUpTime;
        duration = other.duration;
        width = other.width;
    }
    return *this;
}

std::vector<std::unique_ptr<Attack>> Laser::doAttack(const sf::Vector2f &position, const sf::Vector2f &target)
{
    std::vector<std::unique_ptr<Attack>> attacks;
    if (weaponClock.getElapsedTime().asSeconds() >= (1 / attackSpeed))
    {
        weaponClock.restart();
        sf::Texture *beamTexture = nullptr;

        if (attackTextureID == AttackTextureType::Player)
            beamTexture = &ResourceManager::getTexture(TextureType::PlayerBeam);

        else if (attackTextureID == AttackTextureType::Enemy)
            beamTexture = &ResourceManager::getTexture(TextureType::EnemyBeam);

        else if (attackTextureID == AttackTextureType::Boss)
            beamTexture = &ResourceManager::getTexture(TextureType::BossBeam);

        else 
            throw ConfigurationException("Invalid AttackTextureType in Gun::doAttack");

        sf::Vector2f direction = target - position;
        if (direction != sf::Vector2f(0, 0))
            direction = direction.normalized();

        sf::Vector2f origin = position + direction * 70.f; 

        attacks.push_back(std::make_unique<Beam>(origin, *beamTexture, damage, direction, chargeUpTime, duration, width));
    }
    return attacks;
}

void Laser::printDetails(std::ostream &os) const
{
    Weapon::printDetails(os);
    os << ", Laser: (Charge Up Time: " << chargeUpTime
       << ", Duration: " << duration
       << ", Width: " << width << ")";
}
