#pragma once

#include "../headers/Projectile.h"
#include "../headers/ResourceManager.h"

class Weapon
{
    std::string name;
    const sf::Texture *texture;
    sf::Sprite sprite;

protected:
    int damage;
    float attackSpeed;
    sf::Clock weaponClock;

    virtual void printDetails(std::ostream &) const;
    virtual std::vector<std::unique_ptr<Attack>> doFire(const sf::Vector2f &, const sf::Vector2f &) = 0;

public:
    Weapon(const std::string &, const int, const float, const float, const sf::Texture &);
    Weapon(const Weapon &);
    Weapon &operator=(const Weapon &);
    virtual ~Weapon() = default;
    friend std::ostream &operator<<(std::ostream &, const Weapon &);

    std::vector<std::unique_ptr<Attack>> fire(const sf::Vector2f &, const sf::Vector2f &);

    void load(sf::Vector2f &);
    void reset();
    void update(sf::Vector2f &, const sf::Angle &);
    void draw(sf::RenderWindow &);
};