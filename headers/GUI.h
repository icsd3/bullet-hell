#pragma once

#include "../headers/Minimap.h"
#include "../headers/Player.h"

class GUI
{
    std::string fontPath;
    sf::Font font;
    sf::Text health;
    sf::RectangleShape maxHealthBar;
    sf::RectangleShape currentHealthBar;
    Player *player = nullptr;

    sf::Text fps;
    sf::Clock fpsClock;
    int frameCount = 0;
    Minimap minimap;

public:
    GUI();
    GUI(const GUI &) = delete;
    GUI &operator=(const GUI &) = delete;
    ~GUI() = default;
    friend std::ostream &operator<<(std::ostream &, const GUI &);

    void load(const int[5][7], Player &);
    void update(const int &);
    void draw(sf::RenderWindow &);
};