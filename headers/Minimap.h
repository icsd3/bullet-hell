#pragma once

#include "../headers/Utils.h"

#include <iostream>

class Minimap
{
    sf::RectangleShape background;
    std::pair<int, int> currentPos;
    std::vector<sf::RectangleShape> grid;
    std::vector<bool> revealed;
    int mapData[5][7];

    void revealNeighbors(int r, int c);

public:
    Minimap();
    Minimap(const Minimap &) = delete;
    Minimap &operator=(const Minimap &) = delete;
    ~Minimap() = default;

    void load(const int[5][7]);
    void draw(sf::RenderWindow &);
    void update(const int &);
    friend std::ostream &operator<<(std::ostream &, const Minimap &);
};