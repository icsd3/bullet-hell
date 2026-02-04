#pragma once

#include "../headers/Attack.h"

class Slash : public Attack
{
    float duration;

    void doLoad() override;
    bool doUpdate(const float &) override;

public:
    Slash(const sf::Vector2f &, sf::Texture &, const int &, const sf::Vector2f &, const float &, const float &);
    Slash(const Slash &);
    Slash &operator=(const Slash &);
    friend std::ostream &operator<<(std::ostream &, const Slash &);
};