#pragma once

#include "../headers/GameStates.h"
#include "../headers/AssetLoader.h"
#include "../headers/Player.h"
#include "../headers/GUI.h"
#include "../headers/Projectiles.h"
#include "../headers/Enemies.h"

#include <SFML/Graphics.hpp>
#include <optional>

class Game
{
    Game();
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    AssetLoader &loader = AssetLoader::getInstance();
    Player &player = Player::getInstance(target, loader.getPlayerTexture());
    GUI &gui = GUI::getInstance();
    
    std::vector<Projectile> playerProjectiles;
    std::vector<Projectile> enemyProjectiles;
    std::vector<Enemy> enemies;

    sf::RenderWindow window;

    gameStates currentState;

    //bool paused = false;

    sf::Clock weaponClock;
    sf::Clock updateClock;
    sf::Vector2f target;

    void setup();

    bool handleInputs();
    bool handleMainMenuInput(const sf::Event &event);
    void handleAugmentInput(const sf::Event &event);
    void handleLevelInput(const sf::Event &event);

    void draw();
    void drawLevel();
    void drawGUI();
    // void drawDefeat();
    // void drawVictory();

    void handleNewState();

    void spawnEnemies(const int &);
    void updateEntities();
    bool checkEnemyHits(const Projectile &);

public:
    void Play();
    static Game &getInstance();
    ~Game() = default;

    friend std::ostream &operator<<(std::ostream &os, const Game &game);
};