#pragma once

#include "../headers/Enemies.h"
#include "../headers/Projectiles.h"
#include "../headers/Player.h"
#include "../headers/Door.h"

#include <SFML/Graphics.hpp>
#include <iostream>

class Room
{
    Player &player = Player::getInstance();

    sf::Sprite backgroundSprite;

    std::vector<Object> walls;
    std::vector<Door> doors;
    const sf::Texture *doorVertical;
    const sf::Texture *doorHorizontal;
    // std::vector<Object> obstacles;
    Room *up;
    Room *right;
    Room *down;
    Room *left;
    sf::Clock animationClock;
    bool open = false;

    sf::Texture *enemyTexture;
    sf::Texture *playerProjectileTexture;
    sf::Texture *enemyProjectileTexture;

    std::vector<Enemy> enemies; 
    std::vector<Projectile> playerProjectiles;
    std::vector<Projectile> enemyProjectiles;

    void animate(const int &);

public:
    Room(const sf::Texture &, const sf::Texture &, const sf::Texture &, sf::Texture &, sf::Texture &, sf::Texture &);
    Room(const Room &) = default;
    Room &operator=(const Room &) = default;
    ~Room() = default;
    friend std::ostream &operator<<(std::ostream &, const Room &);

    void load(Room *, Room *, Room *, Room *);
    void start();
    void draw(sf::RenderWindow &);
    std::pair<int, Room*> update(const float &);
    bool checkEnemyHits(const Projectile &);
    void spawnPlayerProjectile(const sf::Vector2f &);
    bool checkPlayerHits(const Projectile &);
    int checkPlayerCollisions();
    bool checkEntityCollisions(const Entity &);
};