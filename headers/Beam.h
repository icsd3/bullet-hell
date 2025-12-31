#pragma once

#include "../headers/Attack.h"

class Beam : public Attack
{
    float chargeUpTime;
    float duration;
    float width;

    sf::RectangleShape chargeUp;

    void doLoad() override;
    bool doUpdate(const float &) override;
    void doDraw(sf::RenderWindow &) const override;
    int doHits(const Entity&) const override;

public:
    Beam(const sf::Vector2f &, sf::Texture &, const int &, const sf::Vector2f &, const float &, const float &, const float &);
    Beam(const Beam &);
    Beam &operator=(const Beam &);
    friend std::ostream &operator<<(std::ostream &os, const Beam &beam);
};