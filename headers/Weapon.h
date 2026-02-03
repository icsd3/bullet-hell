#pragma once

#include "../headers/Projectile.h"

#include <fstream>
#include <nlohmann/json.hpp>

enum class AttackTextureType
{
    Player,
    Enemy,
    Boss
};

class Weapon
{
protected:
    std::string name;
    const sf::Texture *texture;
    sf::Sprite sprite;
    int damage;
    float attackSpeed;
    sf::Clock weaponClock;
    AttackTextureType attackTextureID;
    float range;

    virtual void printDetails(std::ostream &) const;
    virtual std::vector<std::unique_ptr<Attack>> doAttack(const sf::Vector2f &, const sf::Vector2f &) = 0;

public:
    Weapon(const std::string &, const int, const float, const float, const sf::Texture &, const AttackTextureType, const float);
    Weapon(const Weapon &);
    Weapon &operator=(const Weapon &);
    virtual ~Weapon() = default;
    friend std::ostream &operator<<(std::ostream &, const Weapon &);

    std::vector<std::unique_ptr<Attack>> attack(const sf::Vector2f &, const sf::Vector2f &);

    void load(sf::Vector2f &);
    void reset();
    void update(sf::Vector2f &, const sf::Angle &);
    void draw(sf::RenderWindow &);
    
    struct WeaponStats
    {
        std::string name;
        int damage;
        float attackSpeed;
        float range;
        const sf::Texture* texture;
    };

    WeaponStats getStats() const;
    const std::string& getName() const;
};