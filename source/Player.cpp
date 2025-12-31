#include "../headers/Player.h"

Player::Player()
    : Entity({LOGICAL_WIDTH * 0.5f, LOGICAL_HEIGHT * 0.8f}, ResourceManager::getTexture(TextureType::Player), 400.f, 100), currentWeapon(0)
{
    weapons.emplace_back(std::make_unique<Gun>("json/Guns.json", 0, 0.f, AttackTextureType::Player));
    weapons.emplace_back(std::make_unique<Gun>("json/Guns.json", 1, 0.f, AttackTextureType::Player));
    weapons.emplace_back(std::make_unique<Gun>("json/Guns.json", 2, 0.f, AttackTextureType::Player));
    weapons.emplace_back(std::make_unique<Gun>("json/Guns.json", 3, 0.f, AttackTextureType::Player));
    weapons.emplace_back(std::make_unique<Gun>("json/Guns.json", 4, 0.f, AttackTextureType::Player));
    weapons.emplace_back(std::make_unique<Sword>("json/Swords.json", 0, AttackTextureType::Player));
    weapons.emplace_back(std::make_unique<Laser>("json/Lasers.json", 0, 0.f, AttackTextureType::Player));
}

void Player::load()
{
    Entity::load({60.f, 0.f}, {0.5f, 0.5f}, {0.6f, 0.6f}, {0.5f, 1.0f}, {0.f, 0.5f}, 6, {
        {4.5f / 14, 0.f}, 
        {9.5f / 14, 0.f}, 
        {1.f, 4.5f / 14}, 
        {1.f, 1.f}, 
        {0.f, 1.f}, 
        {0.f, 4.5f / 14}
    });

    if (currentWeapon >= weapons.size())
        throw OutOfBoundsException("Invalid currentWeapon index in Player::load");

    for (auto &weapon : weapons)
    {
        weapon->load(position);
        weapon->reset();
    }
}

void Player::update(const float &dt, const sf::Vector2f &target, const sf::Vector2f &mousePosition)
{
    if (dt > 0)
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
}

std::vector<std::unique_ptr<Attack>> Player::attack(const sf::Vector2f &target) const
{
    std::vector<std::unique_ptr<Attack>> attacks;

    if (currentWeapon >= weapons.size())
        throw OutOfBoundsException("Invalid currentWeapon index in Player::attack");

    attacks = weapons[currentWeapon]->attack(position, target);

    return attacks;
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

void Player::cycleWeapon(bool forward)
{
    if (forward)
        currentWeapon = (currentWeapon + 1) % weapons.size();
    else
        currentWeapon = (currentWeapon + weapons.size() - 1) % weapons.size();
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