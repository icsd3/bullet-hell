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

    std:: string fontPath = "fonts/arial.ttf";
    sf::Font font;
    sf::Text health;

public:
    static GUI& getInstance();
    ~GUI() = default;
    friend std::ostream& operator<<(std::ostream& os, const GUI& gui);

    void loadGUI();
    void updateGUI(sf::Vector2i);
    void draw(sf::RenderWindow&);
}; 