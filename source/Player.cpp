#include "../headers/Player.h"

Player::Player()
    : Entity({LOGICAL_WIDTH * 0.5f, LOGICAL_HEIGHT * 0.8f}, TextureManager::getPlayerTexture(), 400.f, 100), currentWeapon(0)
{
    
    std::ifstream file("json/Guns.json");
    nlohmann::json data;
    file >> data;
    const auto &w = data[5];
    weapons.emplace_back(std::make_unique<Gun>(
        w["name"],
        w["damage"],
        w["fire_rate"],
        0.f,
        w["bullet_nr"],
        w["spread_angle"],
        w["range"],
        w["bullet_speed"],
        1));
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
        os << "\n        Weapon " << i + 1 << ": " << player.weapons[i];
    }
    os << "\n";
    return os;
}

void Player::update(const float &dt, const sf::Vector2f &target)
{
    sf::Vector2f dir = target - position;
    float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (distance > 5.0f)
    {
        dir /= distance;

        sf::Vector2f moveVec(dir * speed * dt);
        transform(moveVec, 0.f, sf::Angle(sf::degrees(0.f)));
    }

    weapons[currentWeapon]->update();
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

    weapons[currentWeapon]->reset();
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

    bullets = weapons[currentWeapon]->fire(position, target);

    return bullets;
}