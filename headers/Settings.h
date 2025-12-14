#pragma once

#include "../headers/Utils.h"

#include <iostream>

class Settings
{
    Settings();
    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;

    sf::RectangleShape settingsBox;
    sf::RectangleShape screenButton;
    sf::RectangleShape controlsButton;
    sf::Font font;
    sf::Text screenText;
    sf::Text controlsText;

public:
    static Settings &getInstance();
    ~Settings() = default;

    void load();
    void draw(sf::RenderWindow &);
    int handleInput(const sf::RenderWindow &, const sf::Event &);

    friend std::ostream &operator<<(std::ostream &, const Settings &);
};