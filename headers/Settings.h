#pragma once

#include "../headers/Utils.h"
#include "../headers/ResourceManager.h"
#include "../headers/Singleton.h"

#include <iostream>

class Settings : public Singleton<Settings>
{
    sf::RectangleShape settingsBox;
    sf::RectangleShape screenButton;
    sf::RectangleShape controlsButton;
    sf::RectangleShape framerateButton;

    sf::Text screenText;
    sf::Text controlsText;
    sf::Text framerateText;

protected:
    Settings();

public:
    void load();
    void draw(sf::RenderWindow &);
    int handleInput(const sf::RenderWindow &, const sf::Event &);

    friend std::ostream &operator<<(std::ostream &, const Settings &);
};