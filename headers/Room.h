#pragma once

#include "../headers/Enemies.h"
#include "../headers/Projectiles.h"
#include "../headers/Player.h"

#include <SFML/Graphics.hpp>
#include <iostream>

class Room
{
    sf::Sprite backgroundSprite;

    std::vector<sf::RectangleShape> walls;
    std::vector<sf::RectangleShape> doors;
    std::optional<sf::Sprite> doorUp;
    std::optional<sf::Sprite> doorRight;
    std::optional<sf::Sprite> doorDown;
    std::optional<sf::Sprite> doorLeft;
    // std::vector<Object> obstacles;
    Room *up;
    Room *right;
    Room *down;
    Room *left;
    sf::Clock animationClock;
    bool open = false;

public:
    Room(const bool &, const bool &, const bool &, const bool &, const sf::Texture &, const sf::Texture &, const sf::Texture &);
    Room(const Room &) = default;
    Room &operator=(const Room &) = default;
    ~Room() = default;

    void load(Room *, Room *, Room *, Room *);
    void draw(sf::RenderWindow &);
    std::pair<int, Room*> update(const Player &, const bool &);
    int checkCollisions(const Player &);
};