#pragma once

#include "../headers/Object.h"

class Entity : public Object
{
    sf::ConvexShape hitBox;

    virtual bool doTakeDamage(const int &);

    using Object::sprite;

protected:
    float speed;
    int maxHealth;
    int currentHealth;
    // sf::Clock animationClock;

    void doDraw(sf::RenderWindow &) const override;

    void transform(const sf::Vector2f &, const bool &, const sf::Angle &);

    virtual int doHits(const Entity &) const;

    Entity(const sf::Vector2f &, const sf::Texture &, const float, const int);
    Entity(const Entity &);
    Entity &operator=(const Entity &);

public:
    friend std::ostream &operator<<(std::ostream &, const Entity &);

    void load(const ObjectLoadParams &, const int &, const std::vector<sf::Vector2f> &);
    int hits(const Entity &) const;
    bool takeDamage(const int &);
};