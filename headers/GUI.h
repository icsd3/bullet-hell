#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>

class GUI
{
    GUI();
    GUI(const GUI &) = delete;
    GUI &operator=(const GUI &) = delete;

    std::string fontPath = "fonts/arial.ttf";
    sf::Font font;
    sf::Text health;
    sf::RectangleShape maxHealthBar;
    sf::RectangleShape currentHealthBar;
    sf::Text fps;
    sf::Clock fpsClock;
    int frameCount = 0;

public:
    static GUI &getInstance();
    ~GUI() = default;
    friend std::ostream &operator<<(std::ostream &os, const GUI &gui);

    void loadGUI(sf::Window &);
    void updateGUI(const sf::Vector2i &, sf::RenderWindow &);
    void draw(sf::RenderWindow &);
};