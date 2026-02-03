#pragma once

#include "../headers/Utils.h"
#include "../headers/ResourceManager.h"
#include "../headers/Singleton.h"

#include <iostream>

class GameOverMenu : public Singleton<GameOverMenu>
{
    sf::RectangleShape backgroundBox;
    sf::Text titleText;
    sf::Sprite mainMenuButtonSprite;

protected:
    GameOverMenu();

public:
    void load(bool victory);
    void draw(sf::RenderWindow &);
    int handleInput(const sf::RenderWindow &, const sf::Event &);
};
