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
        sf::Texture *weaponTexture;
        switch (texId)
        {
            case 0:
                weaponTexture = &ResourceManager::getTexture(TextureType::Pistol);
                break;
            case 1:
                weaponTexture = &ResourceManager::getTexture(TextureType::Shotgun);
                break;
            case 2:
                weaponTexture = &ResourceManager::getTexture(TextureType::Rifle);
                break;
            case 3:
                weaponTexture = &ResourceManager::getTexture(TextureType::Sniper);
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
            ProjectileTextureType::Player,
            *weaponTexture));
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
    sf::Angle angle = sf::degrees(0);

    if ((mousePosition - position) != sf::Vector2f(0, 0))
            angle = (mousePosition - position).angle();

    bool facingLeft = true;

    if (angle > sf::degrees(90) || angle < sf::degrees(-90))
        facingLeft = false;

    if (distance > 5.0f)
    {
        dir /= distance;
        sf::Vector2f moveVec(dir * speed * dt);

        transform(moveVec, facingLeft, sf::Angle(sf::degrees(0.f)));
    }

    else
        transform({0.f, 0.f}, facingLeft, sf::Angle(sf::degrees(0.f)));

    if (currentWeapon >= weapons.size())
        throw OutOfBoundsException("Invalid currentWeapon index in Player::update");

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

void Player::setPosition(const sf::Vector2f &newPos, const sf::Vector2f &mousePosition)
{
    sf::Vector2f moveVec = newPos - position;
    sf::Angle angle = sf::degrees(0);
    bool facingLeft = true;

    if ((mousePosition - newPos) != sf::Vector2f(0, 0))
        angle = (mousePosition - newPos).angle();

    if (angle > sf::degrees(90) || angle < sf::degrees(-90))
        facingLeft = false;

    transform(moveVec, facingLeft, sf::Angle(sf::degrees(0.f)));
}

std::vector<Projectile> Player::fire(const sf::Vector2f &target) const
{
    std::vector<Projectile> bullets;

    if (currentWeapon >= weapons.size())
        throw OutOfBoundsException("Invalid currentWeapon index in Player::fire");

    bullets = weapons[currentWeapon]->fire(position, target);

    return bullets;
}