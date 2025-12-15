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

    std::string enemyPath;
    std::string bossPath;
    std::string enemyProjectilePath;
    std::string bossProjectilePath;
    std::string roomBackgroundPath;
    std::string doorVerticalPath;
    std::string doorHorizontalPath;
    std::string obstaclePath;

    sf::Texture enemyTexture;
    sf::Texture bossTexture;
    sf::Texture enemyProjectileTexture;
    sf::Texture bossProjectileTexture;
    sf::Texture roomBackgroundTexture;
    sf::Texture doorVerticalTexture;
    sf::Texture doorHorizontalTexture;
    sf::Texture obstacleTexture;

    int map[5][7];
    std::vector<std::shared_ptr<Room>> rooms;
    std::shared_ptr<Room> currentRoom;

    void generateRooms(const int);

public:
    Level(Player &, GUI &);
    friend std::ostream &operator<<(std::ostream &, const Level &);

    void update(const float &, sf::Vector2f &);
    void load(const int);
    std::pair<int, sf::Vector2f> handleInput(const sf::Event &, const bool &, const sf::RenderWindow &);
    sf::Vector2f handleMovementInput(const bool &, const sf::RenderWindow &);
    static sf::Vector2f handleShootInput(const sf::RenderWindow &);
    void playerFire(const sf::Vector2f &);
    void draw(sf::RenderWindow &);
};