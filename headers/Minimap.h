#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>

class Minimap
{
    Minimap();
    sf::RectangleShape background;
    std::pair<int, int> currentPos;
    std::vector<sf::RectangleShape> grid;

public:
    static Minimap &getInstance();
    ~Minimap() = default;

    void load(const int [5][7]);
    void draw(sf::RenderWindow &);
    void update(const int &);
    friend std::ostream &operator<<(std::ostream &, const Minimap &);
};