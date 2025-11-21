#pragma once

#include "../headers/MainMenu.h"
#include "../headers/AssetLoader.h"
#include "../headers/Player.h"
#include "../headers/GUI.h"
#include "../headers/Projectiles.h"
#include "../headers/Enemies.h"
#include "../headers/Settings.h"
#include "../headers/Augment.h"

#include <SFML/Graphics.hpp>
#include <optional>

enum gameStates
{
    main_menu = 1,
    augment_1 = 2,
    level_1 = 3,
    augment_2 = 4,
    level_2 = 5,
    augment_3 = 6,
    level_3 = 7,
    defeat = 8,
    victory = 9
};

class Game
{
    Game();
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    Settings &settings = Settings::getInstance();

    sf::RenderWindow window;

    gameStates currentState;

    bool paused = false;
    bool openSettings = false;
    bool fullscreen = true;
    bool controls = true; //true for mouse, false for WASD
    sf::Clock updateClock;
    sf::Vector2f target;

    mainMenu &menu = mainMenu::getInstance();
    Augment &augment = Augment::getInstance();

    AssetLoader &loader = AssetLoader::getInstance();
    Player &player = Player::getInstance(target, loader.getPlayerTexture());
    GUI &gui = GUI::getInstance();
    std::vector<Projectile> playerProjectiles;
    std::vector<Projectile> enemyProjectiles;
    std::vector<Enemy> enemies;

    void setup();
    static void selectGameState(gameStates &gameState);

    bool handleInputs();
    std::pair<int, sf::Vector2f> handleLevelInput(const sf::Event &event);

    void draw();
    void drawLevel();
    void drawGUI();
    // void drawDefeat();
    // void drawVictory();

    void handleNewState();

    void spawnEnemies(const int &);
    void updateEntities();
    bool checkEnemyHits(const Projectile &);

    void togglePause();

public:
    void Play();
    static Game &getInstance();
    ~Game() = default;

    friend std::ostream &operator<<(std::ostream &os, const Game &game);
};