#pragma once

#include "../headers/Drop.h"

class WeaponDrop : public Drop
{
    std::unique_ptr<Weapon> weapon;
    bool doApplyEffect(Player &) override;

public:
    explicit WeaponDrop(const sf::Vector2f &);
    
    void load() override; 
};
