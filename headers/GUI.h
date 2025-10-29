#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>

#include <iostream>

class GUI
{
    GUI();
    GUI(const GUI&) = delete;
    GUI& operator=(const GUI&) = delete;

    friend class Game;

    sf::Font font;
    sf::Text health;

    void loadGUI();
    void updateGUI(int, int);

public:
    static GUI& getInstance();
    ~GUI() = default;
}; 