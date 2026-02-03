#include "../headers/WeaponDrop.h"

WeaponDrop::WeaponDrop(const sf::Vector2f &pos)
    : Drop(pos, ResourceManager::getTexture(TextureType::Weapon))
{
    std::mt19937 &rng = Utils::getRng();
    
    auto getRandomWeaponIndex = [&rng](const int& startIdx, const std::string& file_path) -> size_t 
    {
        std::ifstream file(file_path);
        if (!file.is_open())
            throw ConfigurationException("Failed to open " + file_path);
        
        nlohmann::json data;
        file >> data;
        size_t weaponCount = data.size();
        
        std::uniform_int_distribution<size_t> weaponDist(startIdx, weaponCount - 1);
        return weaponDist(rng);
    };
    
    std::uniform_int_distribution<int> typeDist(0, 3);
    int type = typeDist(rng);

    std::string weaponFile;

    switch (type)
    {
    case 0:
        weaponFile = "json/Guns.json";
        weapon = std::make_unique<Gun>(weaponFile, getRandomWeaponIndex(1, weaponFile), 0.f, AttackTextureType::Player);
        break;
    case 1:
        weaponFile = "json/Lasers.json";
        weapon = std::make_unique<Laser>(weaponFile, getRandomWeaponIndex(0, weaponFile), 0.f, AttackTextureType::Player);
        break;
    case 2:
        weaponFile = "json/Spears.json";
        weapon = std::make_unique<Spear>(weaponFile, getRandomWeaponIndex(0, weaponFile), AttackTextureType::Player);
        break;
    case 3:
        weaponFile = "json/Swords.json";
        weapon = std::make_unique<Sword>(weaponFile, getRandomWeaponIndex(1, weaponFile), AttackTextureType::Player);
        break;
    default:
        throw ConfigurationException("Invalid weapon type");
    }
}

void WeaponDrop::load()
{
    sf::Vector2f scaleFactor = {64.f, 0.f};
    sf::Vector2f spriteOriginFactor = {0.5f, 0.5f};
    sf::Vector2f collisionBoxSizeFactor = {0.6f, 0.6f}; 
    sf::Vector2f collisionBoxOriginFactor = {0.5f, 0.5f};
    sf::Vector2f collisionBoxPositionFactor = {0.f, 0.f};
    ObjectLoadParams params(scaleFactor, spriteOriginFactor, collisionBoxSizeFactor, collisionBoxOriginFactor, collisionBoxPositionFactor);
    Object::load(params);
}

bool WeaponDrop::doApplyEffect(Player &player)
{
    player.addWeapon(std::move(weapon));
    return true;
}
