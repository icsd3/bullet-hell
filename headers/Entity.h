#pragma once

#include "../headers/Object.h"

class Entity : public Object
{
protected:
    float speed;
    int maxHealth;
    int currentHealth;
    sf::ConvexShape hitBox;
    // sf::Clock animationClock;

    void doDraw(sf::RenderWindow &) override;

    virtual bool doTakeDamage(const int &);
    virtual int doHits(const Entity &other) const;

    Entity(const sf::Vector2f &, sf::Texture &, const float, const int);
    Entity(const Entity &);
    Entity &operator=(const Entity &);

public:
    friend std::ostream &operator<<(std::ostream &, const Entity &);

    void load(const float &, const sf::Vector2f &, const sf::Vector2f &, const sf::Vector2f &, const int &, const std::vector<sf::Vector2f> &);
    int hits(const Entity &other) const;
    bool takeDamage(const int &);
};