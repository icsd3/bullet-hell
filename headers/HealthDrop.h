#pragma once

#include "../headers/Drop.h"

class HealthDrop : public Drop
{
    int healAmount;
    bool doApplyEffect(Player &) override;

public:
    HealthDrop(const sf::Vector2f &);
};
