#pragma once

#include "../headers/Enemies.h"
#include "../headers/Projectiles.h"
#include "../headers/Player.h"
#include "../headers/Door.h"

#include <SFML/Graphics.hpp>
#include <iostream>

class Room
{
    sf::Sprite backgroundSprite;

    std::vector<Object> walls;
    std::vector<Door> doors;
    const sf::Texture &doorVertical;
    const sf::Texture &doorHorizontal;
    // std::vector<Object> obstacles;
    Room *up;
    Room *right;
    Room *down;
    Room *left;
    sf::Clock animationClock;
    bool open = false;

    void animate(const int &);

public:
    Room(const sf::Texture &, const sf::Texture &, const sf::Texture &);
    Room(const Room &) = default;
    Room &operator=(const Room &) = default;
    ~Room() = default;

    void load(Room *, Room *, Room *, Room *);
    void draw(sf::RenderWindow &);
    std::pair<int, Room*> update(const Player &, const bool &);
    int checkCollisions(const Player &);
    friend std::ostream &operator<<(std::ostream &, const Room &);
};