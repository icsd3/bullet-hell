#pragma once

#include "../headers/EnemyRoom.h"
#include "../headers/Utils.h"
#include "../headers/GUI.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>

class Level
{
    Level();

    std::string playerTexturePath;
    std::string enemyPath;
    std::string playerProjectilePath;
    std::string enemyProjectilePath;
    std::string roomBackgroundPath;
    std::string doorVerticalPath;
    std::string doorHorizontalPath;
    std::string obstaclePath;

    sf::Texture playerTexture;
    sf::Texture enemyTexture;
    sf::Texture playerProjectileTexture;
    sf::Texture enemyProjectileTexture;
    sf::Texture roomBackgroundTexture;
    sf::Texture doorVerticalTexture;
    sf::Texture doorHorizontalTexture;
    sf::Texture obstacleTexture;

    int map[5][7];
    std::vector<std::shared_ptr<Room>> rooms;
    std::shared_ptr<Room> currentRoom;
    GUI &gui = GUI::getInstance();
    Player &player = Player::Initialize(playerTexture, playerProjectileTexture);

public:
    static Level &getInstance();
    friend std::ostream &operator<<(std::ostream &, const Level &);

    void generateRooms(const int);
    void update(const float &, sf::Vector2f &);
    void load(const int);
    std::pair<int, sf::Vector2f> handleInput(const sf::Event &, const bool &, const sf::RenderWindow &);
    sf::Vector2f handleMovementInput(const bool &, const sf::RenderWindow &);
    static sf::Vector2f handleShootInput(const sf::RenderWindow &);
    void playerFire(const sf::Vector2f &);
    void draw(sf::RenderWindow &);
};