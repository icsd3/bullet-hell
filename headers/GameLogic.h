#pragma once

#include "../headers/MainMenu.h"
#include "../headers/Level.h"
#include "../headers/Settings.h"
#include "../headers/Augment.h"

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

    bool openSettings = false;
    bool fullscreen = true;
    bool controls = true; // true for WASD, false for mouse
    sf::Clock updateClock;
    sf::Vector2f target;
    gameStates currentState;

    mainMenu &menu = mainMenu::getInstance();

    std::string playerPath;
    std::string playerProjectilePath;

    sf::Texture playerTexture;
    sf::Texture playerProjectileTexture;

    std::unique_ptr<Player> player;
    std::unique_ptr<GUI> gui;
    std::unique_ptr<Augment> augment;
    std::unique_ptr<Level> level;

    sf::RenderWindow window;

    void setup();
    static void selectGameState(gameStates &);

    bool handleInputs();

    void draw();

    void handleNewState();

    void togglePause();

public:
    void Play();
    static Game &getInstance();
    ~Game() = default;

    friend std::ostream &operator<<(std::ostream &, const Game &);
};