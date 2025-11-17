#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Settings
{
    Settings();
    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;

    bool fullscreen;
    bool controls;

    sf::RectangleShape settingsBox;
    sf::Font font;
    sf::Text screenText;
    sf::Text controlsText;

public:
    static Settings &getInstance();
    ~Settings() = default;

    void loadSettingsBox(sf::RenderWindow &, const bool &, const bool &);
    void draw(sf::RenderWindow &, const bool &, const bool &);

    friend std::ostream &operator<<(std::ostream &os, const Settings &settings);
};