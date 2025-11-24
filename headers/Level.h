#pragma once

#include "../headers/Room.h"

#include <SFML/Graphics.hpp>
#include <iostream>

class Level
{
    Level();

    std::string enemyPath;
    std::string playerProjectilePath;
    std::string enemyProjectilePath;

    sf::Texture enemyTexture;
    sf::Texture playerProjectileTexture;
    sf::Texture enemyProjectileTexture;

    std::vector<Enemy> enemies; 
    std::vector<Projectile> playerProjectiles;
    std::vector<Projectile> enemyProjectiles;

    Room currentRoom;
    Player &player = Player::getInstance();

public:
    static Level &getInstance();
    void spawnEnemies(const int &);
    void updateEntities(const float &);
    bool checkEnemyHits(const Projectile &);
    bool checkPlayerHits(const Projectile &, Player &);
    void load();
    void spawnPlayerProjectile(const sf::Vector2f &);
    std::pair<int, sf::Vector2f> handleInput(const sf::Event &, const bool &, const sf::RenderWindow &);
    sf::Vector2f handleMovementInput(const bool &, const sf::RenderWindow &);
    sf::Vector2f handleShootInput(const sf::RenderWindow &);
    void draw(sf::RenderWindow &);
};