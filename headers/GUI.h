#pragma once

#include "../headers/Minimap.h"
#include "../headers/Player.h"

class GUI
{
    sf::Text health;
    sf::RectangleShape maxHealthBar;
    sf::RectangleShape currentHealthBar;
    
    sf::Text weaponName;
    sf::RectangleShape weaponNameBox;

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

    void load(const int[MAP_SIZE_Y][MAP_SIZE_X], Player &);
    void update(const int &);
    void draw(sf::RenderWindow &);
};