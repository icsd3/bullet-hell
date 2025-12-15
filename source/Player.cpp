#include "../headers/Player.h"

Player::Player(sf::Texture &tex, sf::Texture &prtex)
    : Entity({LOGICAL_WIDTH * 0.5f, LOGICAL_HEIGHT * 0.8f}, tex, 0.2f, 100), currentWeapon(nullptr)
{
    std::ifstream file("json/Weapons.json");
    nlohmann::json data;
    file >> data;
    const auto &w = data[5];
    weapons.emplace_back(Weapon(
        w["name"],
        w["damage"],
        w["bullet_nr"],
        w["fire_rate"],
        w["spread_angle"],
        w["range"],
        w["bullet_speed"],
        0.f,
        prtex));
    currentWeapon = &weapons.back();
}

std::ostream &operator<<(std::ostream &os, const Player &player)
{
    os << "Player:\n    ";
    os << static_cast<const Entity &>(player);
    os << "\n";
    os << "    Max Health: " << player.maxHealth << "\n";
    os << "    Current Health: " << player.currentHealth << "\n";
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
        sprite.move(sf::Vector2f(dir * speed * LOGICAL_WIDTH * dt));
        collisionBox.move(sf::Vector2f(dir * speed * LOGICAL_WIDTH * dt));
        hitBox.move(sf::Vector2f(dir * speed * LOGICAL_WIDTH * dt));
        position = sprite.getPosition();
        if (dir.x > 0.f)
        {
            sprite.setScale(sf::Vector2f(-std::abs(sprite.getScale().x), sprite.getScale().y));
        }
        else if (dir.x < 0.f)
        {
            sprite.setScale(sf::Vector2f(std::abs(sprite.getScale().x), sprite.getScale().y));
        }
    }

    currentWeapon->update();
}

void Player::load()
{
    Entity::load(60.f, {0.6f, 0.6f}, {0.5f, 1.0f}, {0.f, 0.5f}, 6, {{4.5f / 14, 0.f}, {9.5f / 14, 0.f}, {1.f, 4.5f / 14}, {1.f, 1.f}, {0.f, 1.f}, {0.f, 4.5f / 14}});

    currentWeapon->reset();
}

sf::Vector2i Player::getHealthStatus() const
{
    return {currentHealth, maxHealth};
}

sf::Vector2f Player::getPosition() const
{
    return sprite.getPosition();
}

void Player::setPosition(const sf::Vector2f &newPos)
{
    sf::Vector2f offset = newPos - position;
    position = newPos;
    sprite.setPosition(newPos);
    collisionBox.move(offset);
    hitBox.move(offset);
}

std::vector<Projectile> Player::fire(const sf::Vector2f &target) const
{
    std::vector<Projectile> bullets;

    bullets = currentWeapon->fire(position, target);

    return bullets;
}

bool Player::doTakeDamage(const int &dmg)
{
    currentHealth -= dmg;
    if (currentHealth <= 0)
    {
        currentHealth = 0;
        return true;
    }
    return false;
}