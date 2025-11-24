#pragma once

#include "../headers/Enemies.h"
#include "../headers/Projectiles.h"
#include "../headers/Player.h"

#include <SFML/Graphics.hpp>
#include <iostream>

class Room
{
    std::string backgroundPath;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    // std::vector<Object> walls;
    // std::vector<Object> obstacles;

public:
    Room();
    Room(const Room &) = default;
    Room &operator=(const Room &) = default;
    ~Room() = default;

    void load();
    void draw(sf::RenderWindow &);

};