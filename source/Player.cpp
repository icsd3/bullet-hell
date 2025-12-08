#include "../headers/Player.h"
#include "../headers/Utils.h"

Player::Player(sf::Texture &tex, sf::Texture &prtex)
    : Entity({LOGICAL_WIDTH * 0.5f, LOGICAL_HEIGHT * 0.8f}, false, tex, 0.2f, 100), currentWeapon(nullptr)
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
        prtex
    ));
    currentWeapon = &weapons.back();
}

std::unique_ptr<Player> Player::instance = nullptr; 

Player &Player::getInstance()
{
    return *instance;
}

Player &Player::Initialize(sf::Texture &tex, sf::Texture &prtex)
{
    instance = std::unique_ptr<Player>(new Player(tex, prtex));
    return *instance;
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
        sprite.value().move(sf::Vector2f(dir * speed * LOGICAL_WIDTH * dt));
        collisionBox.move(sf::Vector2f(dir * speed * LOGICAL_WIDTH * dt));
        hitBox.move(sf::Vector2f(dir * speed * LOGICAL_WIDTH * dt));
        position = sprite.value().getPosition();
        if (dir.x > 0.f)
        {
            orientation = true;
            sprite.value().setScale(sf::Vector2f(-std::abs(sprite.value().getScale().x), sprite.value().getScale().y));
        }
        else
        {
            orientation = false;
            sprite.value().setScale(sf::Vector2f(std::abs(sprite.value().getScale().x), sprite.value().getScale().y));
        }
    }

    currentWeapon -> update();
}

void Player::doLoad()
{   
    Entity::doLoad();

    currentWeapon->reset();
}

sf::Vector2i Player::getHealthStatus() const
{
    return {currentHealth, maxHealth};
}

sf::Vector2f Player::getPosition() const
{
    return sprite.value().getPosition();
}

void Player::setPosition(const sf::Vector2f &newPos)
{
    sf::Vector2f offset = newPos - position;
    position = newPos;
    sprite.value().setPosition(newPos);
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