#pragma once

#include "../headers/EnemyRoom.h"
#include "../headers/BossRoom.h"
#include "../headers/GUI.h"
#include "../headers/Augment.h"
#include "../headers/Player.h"
#include <queue>

class Level
{
    Level(const Level &) = delete;
    Level &operator=(const Level &) = delete;

    Player &player;
    GUI &gui;
    sf::Vector2f target;
    bool moved = false;

    int map[5][7];
    std::vector<std::shared_ptr<Room>> rooms;
    std::shared_ptr<Room> currentRoom;

    void generateRooms(const int);

public:
    Level(Player &, GUI &);
    friend std::ostream &operator<<(std::ostream &, const Level &);

    int update(const float &, const sf::Vector2f &);
    void load(const int);
    bool handleInput(const sf::Event &, const bool &, const sf::RenderWindow &);
    void handleMovementInput(const bool &, const sf::RenderWindow &);
    void handleShootInput(const sf::RenderWindow &);
    void playerFire(const sf::Vector2f &);
    void draw(sf::RenderWindow &);
};