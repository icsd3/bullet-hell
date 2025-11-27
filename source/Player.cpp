#include "../headers/Player.h"
#include "../headers/Utils.h"

Player::Player(const sf::Texture &tex)
    : Entity(false, {LOGICAL_WIDTH * 0.5f, LOGICAL_HEIGHT * 0.8f}, false, "textures/player.png", tex, 0.2f), maxHealth(100), currentHealth(100), currentWeapon(nullptr), texture(tex)
{
}

std::unique_ptr<Player> Player::instance = nullptr; 

Player &Player::getInstance()
{
    return *instance;
}

Player &Player::Initialize(const sf::Texture &tex)
{
    instance = std::unique_ptr<Player>(new Player(tex));
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
        sprite.move(sf::Vector2f(dir * speed * LOGICAL_WIDTH * dt));
        collisionBox.move(sf::Vector2f(dir * speed * LOGICAL_WIDTH * dt));
        position = sprite.getPosition();
        if (dir.x > 0.f)
        {
            orientation = true;
            sprite.setScale(sf::Vector2f(-std::abs(sprite.getScale().x), sprite.getScale().y));
        }
        else
        {
            orientation = false;
            sprite.setScale(sf::Vector2f(std::abs(sprite.getScale().x), sprite.getScale().y));
        }
    }
}

void Player::load()
{   
    std::ifstream file("json/Weapons.json");
    nlohmann::json data;
    file >> data;
    const auto &w = data[4];
    weapons.emplace_back(Weapon(
        w["name"],
        w["damage"],
        w["bullet_nr"],
        w["fire_rate"],
        w["spread_angle"],
        w["range"],
        w["bullet_speed"],
        0.f
    ));
    currentWeapon = &weapons.back();
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    sprite.setPosition(position);

    float scale = 1.f * LOGICAL_WIDTH / static_cast<float>(texture.getSize().x) / 20.f;
    sprite.setScale(sf::Vector2f(scale, scale));
    
    if (orientation)
    {
        sprite.setScale(sf::Vector2f(-std::abs(sprite.getScale().x), sprite.getScale().y));
    }
    else
    {
        sprite.setScale(sf::Vector2f(std::abs(sprite.getScale().x), sprite.getScale().y));
    }

    collisionBox.setFillColor(sf::Color(0, 0, 255, 100));
    collisionBox.setSize(sf::Vector2f(scale * 0.7f * texture.getSize().x, scale * 0.4f * texture.getSize().y));
    collisionBox.setOrigin(sf::Vector2f(collisionBox.getSize().x / 2.f, collisionBox.getSize().y));
    collisionBox.setPosition(sf::Vector2f(position.x, position.y + scale * 0.5f * texture.getSize().x));

    currentWeapon->reset();
}

void Player::draw(sf::RenderWindow &window)
{
    sf::Sprite drawSprite = sprite;
    drawSprite.setPosition(mapToScreen(position, window));
    sf::Vector2f scaleFactor = getScaleFactor(window);
    drawSprite.scale(scaleFactor);
    window.draw(drawSprite);
    // sf::RectangleShape drawCollisionBox = collisionBox;
    // drawCollisionBox.setPosition(mapToScreen(collisionBox.getPosition(), window));
    // drawCollisionBox.scale(scaleFactor);
    // window.draw(drawCollisionBox);
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
}

std::vector<Projectile> Player::fire(const sf::Vector2f &target, const sf::Texture &tex)
{
    return currentWeapon->fire(position, target, tex);
}

bool Player::takeDamage(const int &dmg)
{
    currentHealth -= dmg;
    if (currentHealth <= 0)
    {
        currentHealth = 0;
        return true;
    }
    return false;
}

bool Player::canFire()
{
    return currentWeapon->canFire();
}

bool Player::boxCollidesWith(const sf::RectangleShape &rect) const
{
    return (collisionBox.getGlobalBounds().findIntersection(rect.getGlobalBounds())).has_value();
}

bool Player::boxCollidesWith(const Object &obj) const
{
    return obj.collidesWith(collisionBox);
}