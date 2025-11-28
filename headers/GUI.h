#pragma once

#include <SFML/Graphics.hpp>
#include "../headers/Minimap.h"

#include <iostream>

class GUI
{
    GUI();
    GUI(const GUI &) = delete;
    GUI &operator=(const GUI &) = delete;

    std::string fontPath;
    sf::Font font;
    sf::Text health;
    sf::RectangleShape maxHealthBar;
    sf::RectangleShape currentHealthBar;
    sf::Text fps;
    sf::Clock fpsClock;
    int frameCount = 0;
    Minimap &minimap = Minimap::getInstance();

public:
    static GUI &getInstance();
    ~GUI() = default;
    friend std::ostream &operator<<(std::ostream &, const GUI &);

    void load();
    void update(const sf::Vector2i &);
    void draw(sf::RenderWindow &);
};