#pragma once

#include "../headers/Utils.h"
#include "../headers/ResourceManager.h"
#include "../headers/Singleton.h"

#include <iostream>

class PauseMenu : public Singleton<PauseMenu>
{
    sf::RectangleShape pauseBox;
    sf::Sprite resumeButtonSprite;
    sf::Sprite settingsButtonSprite;
    sf::Sprite mainMenuButtonSprite;

protected:
    PauseMenu();

public:
    void load();
    void draw(sf::RenderWindow &);
    int handleInput(const sf::RenderWindow &, const sf::Event &);
};
