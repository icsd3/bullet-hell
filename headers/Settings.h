#pragma once

#include "../headers/Utils.h"
#include "../headers/ResourceManager.h"

#include <iostream>

class Settings
{
    Settings();
    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;

    sf::RectangleShape settingsBox;
    sf::RectangleShape screenButton;
    sf::RectangleShape controlsButton;
    sf::RectangleShape framerateButton;

    sf::Text screenText;
    sf::Text controlsText;
    sf::Text framerateText;

public:
    static Settings &getInstance();
    ~Settings() = default;

    void load();
    void draw(sf::RenderWindow &);
    int handleInput(const sf::RenderWindow &, const sf::Event &);

    friend std::ostream &operator<<(std::ostream &, const Settings &);
};