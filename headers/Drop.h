#pragma once

#include "../headers/Player.h"

class Drop : public Object
{
protected:
    virtual bool doApplyEffect(Player &) = 0;

public:
    Drop(const sf::Vector2f &, const sf::Texture &);
    virtual void load();
    bool applyEffect(Player &);
};
