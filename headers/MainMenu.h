#pragma once

#include "../headers/Utils.h"
#include "../headers/ResourceManager.h"
#include "../headers/Singleton.h"

#include <iostream>

class mainMenu : public Singleton<mainMenu>
{
    friend mainMenu& Singleton<mainMenu>::getInstance();
    mainMenu();

    sf::Sprite backgroundSprite;
    sf::Sprite startButtonSprite;
    sf::Sprite settingsButtonSprite;
    sf::Sprite exitButtonSprite;

public:
    void load();
    void draw(sf::RenderWindow &);
    int handleInput(const sf::RenderWindow &, const sf::Event &);
};