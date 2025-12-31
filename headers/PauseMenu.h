#pragma once

#include "../headers/Utils.h"
#include "../headers/ResourceManager.h"

#include <iostream>

class PauseMenu
{
    PauseMenu();
    PauseMenu(const PauseMenu &) = delete;
    PauseMenu &operator=(const PauseMenu &) = delete;

    sf::RectangleShape pauseBox;
    sf::Sprite resumeButtonSprite;
    sf::Sprite settingsButtonSprite;
    sf::Sprite mainMenuButtonSprite;

public:
    static PauseMenu &getInstance();
    ~PauseMenu() = default;

    void load();
    void draw(sf::RenderWindow &);
    int handleInput(const sf::RenderWindow &, const sf::Event &);
};
