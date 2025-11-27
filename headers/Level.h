#pragma once

#include "../headers/Room.h"
#include "../headers/Utils.h"

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

    sf::Texture playerTexture;
    sf::Texture enemyTexture;
    sf::Texture playerProjectileTexture;
    sf::Texture enemyProjectileTexture;
    sf::Texture roomBackgroundTexture;
    sf::Texture doorVerticalTexture;
    sf::Texture doorHorizontalTexture;

    std::vector<Enemy> enemies; 
    std::vector<Projectile> playerProjectiles;
    std::vector<Projectile> enemyProjectiles;

    int map[5][7];
    std::vector<Room> rooms;
    Room *currentRoom = nullptr;
    Player &player = Player::Initialize(playerTexture);

public:
    static Level &getInstance();
    void generateRooms(const int);
    void spawnEnemies(const int &);
    void update(const float &, const sf::Vector2f &);
    bool checkEnemyHits(const Projectile &);
    static bool checkPlayerHits(const Projectile &, Player &);
    void load(const int);
    void spawnPlayerProjectile(const sf::Vector2f &);
    std::pair<int, sf::Vector2f> handleInput(const sf::Event &, const bool &, const sf::RenderWindow &);
    sf::Vector2f handleMovementInput(const bool &, const sf::RenderWindow &);
    static sf::Vector2f handleShootInput(const sf::RenderWindow &);
    void draw(sf::RenderWindow &);
};