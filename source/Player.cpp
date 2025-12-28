#include "../headers/Player.h"

Player::Player()
    : Entity({LOGICAL_WIDTH * 0.5f, LOGICAL_HEIGHT * 0.8f}, ResourceManager::getTexture(TextureType::Player), 400.f, 100), currentWeapon(0)
{
    
    std::ifstream file("json/Guns.json");
    if (!file.is_open())
        throw ConfigurationException("Failed to open json/Guns.json");

    nlohmann::json data;
    try 
    {
        file >> data;
    } 
    catch (const nlohmann::json::parse_error& e) 
    {
        throw ConfigurationException("Failed to parse json/Guns.json: " + std::string(e.what()));
    }

    const auto &w = data[4];
    try 
    {
        int texId = w.at("texture").get<int>();
        sf::Texture *texture;
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
                throw ConfigurationException("json/Guns.json is missing required fields or has invalid types");
                break;
        }
        weapons.emplace_back(std::make_unique<Gun>(
            w.at("name").get<std::string>(),
            w.at("damage").get<int>(),
            w.at("fire_rate").get<float>(),
            0.f,
            w.at("bullet_nr").get<int>(),
            w.at("spread_angle").get<float>(),
            w.at("range").get<float>(),
            w.at("bullet_speed").get<float>(),
            1,
            *texture));
    } 
    catch (const nlohmann::json::exception& e) 
    {
        throw ConfigurationException("json/Guns.json is missing required fields or has invalid types: " + std::string(e.what()));
    }
}

std::ostream &operator<<(std::ostream &os, const Player &player)
{
    os << "Player:\n    ";
    os << static_cast<const Entity &>(player);
    os << "\n";
    os << "    Weapons:\n";
    os << "        Count: " << player.weapons.size();
    for (unsigned int i = 0; i < player.weapons.size(); i++)
    {
        os << "\n        Weapon " << i + 1 << ": " << *player.weapons[i];
    }
    os << "\n";
    return os;
}

void Player::update(const float &dt, const sf::Vector2f &target, const sf::Vector2f &mousePosition)
{
    sf::Vector2f dir = target - position;
    float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (distance > 5.0f)
    {
        dir /= distance;

        sf::Vector2f moveVec(dir * speed * dt);
        transform(moveVec, 0.f, sf::Angle(sf::degrees(0.f)));
    }

    if (currentWeapon >= weapons.size())
        throw OutOfBoundsException("Invalid currentWeapon index in Player::update");

    sf::Angle angle = sf::degrees(0);

    if ((mousePosition - position) != sf::Vector2f(0, 0))
        angle = (mousePosition - position).angle();

    weapons[currentWeapon]->update(position, angle);
}

void Player::load()
{
    Entity::load(60.f, {0.6f, 0.6f}, {0.5f, 1.0f}, {0.f, 0.5f}, 6, {
        {4.5f / 14, 0.f}, 
        {9.5f / 14, 0.f}, 
        {1.f, 4.5f / 14}, 
        {1.f, 1.f}, 
        {0.f, 1.f}, 
        {0.f, 4.5f / 14}
    });

    if (currentWeapon >= weapons.size())
        throw OutOfBoundsException("Invalid currentWeapon index in Player::load");

    weapons[currentWeapon]->load(position);
    weapons[currentWeapon]->reset();
}

void Player::doDraw(sf::RenderWindow &window) const
{
    Entity::doDraw(window);

    if (currentWeapon >= weapons.size())
        throw OutOfBoundsException("Invalid currentWeapon index in Player::doDraw");

    weapons[currentWeapon]->draw(window);
}

sf::Vector2i Player::getHealthStatus() const
{
    return {currentHealth, maxHealth};
}

sf::Vector2f Player::getPosition() const
{
    return position;
}

void Player::setPosition(const sf::Vector2f &newPos)
{
    sf::Vector2f moveVec = newPos - position;
    transform(moveVec, 9999.f, sf::Angle(sf::degrees(0.f)));
}

std::vector<Projectile> Player::fire(const sf::Vector2f &target) const
{
    std::vector<Projectile> bullets;

    if (currentWeapon >= weapons.size())
        throw OutOfBoundsException("Invalid currentWeapon index in Player::fire");

    bullets = weapons[currentWeapon]->fire(position, target);

    return bullets;
}