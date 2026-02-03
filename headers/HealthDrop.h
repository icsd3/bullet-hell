#pragma once

#include "../headers/Drop.h"
#include "../headers/SoundManager.h"

class HealthDrop : public Drop
{
    int healAmount;
    bool doApplyEffect(Player &) override;

public:
    explicit HealthDrop(const sf::Vector2f &);
};
