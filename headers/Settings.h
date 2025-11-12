#pragma once

#include <SFML/Graphics.hpp>

class Settings
{
    Settings();
    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;

    bool fullscreen = true;
    bool controls = true;

    sf::RectangleShape settingsBox;
    sf::Font font;
    sf::Text fullscreenText;
    sf::Text controlsText;

public:
    static Settings &getInstance();
    ~Settings() = default;
    friend std::ostream &operator<<(std::ostream &os, const Settings &settings);

    bool changeFullscreen();
    bool changeControls();
};