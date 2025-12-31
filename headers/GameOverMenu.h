#pragma once

#include "../headers/Utils.h"
#include "../headers/ResourceManager.h"

#include <iostream>

class GameOverMenu
{
    GameOverMenu();
    GameOverMenu(const GameOverMenu &) = delete;
    GameOverMenu &operator=(const GameOverMenu &) = delete;

    sf::RectangleShape backgroundBox;
    sf::Text titleText;
    sf::Sprite mainMenuButtonSprite;

public:
    static GameOverMenu &getInstance();
    ~GameOverMenu() = default;

    void load(bool victory);
    void draw(sf::RenderWindow &);
    int handleInput(const sf::RenderWindow &, const sf::Event &);
};
