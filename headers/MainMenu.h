#pragma once

#include "../headers/Utils.h"
#include "../headers/TextureManager.h"

#include <iostream>

class mainMenu
{
    mainMenu();
    mainMenu(const mainMenu &) = delete;
    mainMenu &operator=(const mainMenu &) = delete;



    sf::Sprite backgroundSprite;
    sf::Sprite startButtonSprite;
    sf::Sprite settingsButtonSprite;
    sf::Sprite exitButtonSprite;

public:
    static mainMenu &getInstance();
    ~mainMenu() = default;

    void load();
    void draw(sf::RenderWindow &);
    int handleInput(const sf::RenderWindow &, const sf::Event &);
};