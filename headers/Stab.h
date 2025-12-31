#pragma once

#include "../headers/Attack.h"

class Stab : public Attack
{
    float duration;
    float stabSpeed;

    void doLoad() override;
    bool doUpdate(const float &) override;

public:
    Stab(const sf::Vector2f &, sf::Texture &, const int &, const sf::Vector2f &, const float &, const float &, const float &);
    Stab(const Stab &);
    Stab &operator=(const Stab &);
    friend std::ostream &operator<<(std::ostream &, const Stab &);
};