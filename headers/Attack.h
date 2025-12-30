#pragma once

#include "../headers/Entity.h"

class Attack : public Entity
{
protected:
    int damage;
    sf::Vector2f direction;
    sf::Vector2f origin;
    float range;    
    mutable std::unordered_set<const Entity *> hitList;

    virtual void doLoad() = 0;
    virtual bool doUpdate(const float &) = 0;

    int doHits(const Entity &other) const override;

public:
    Attack(const sf::Vector2f &, sf::Texture &, float, const int &, const int &, const sf::Vector2f &, const float &);
    friend std::ostream &operator<<(std::ostream &, const Attack &);

    void load();
    bool update(const float &);
};