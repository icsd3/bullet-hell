#pragma once

#include "../headers/MainMenu.h"
#include "../headers/Level.h"
#include "../headers/Settings.h"
#include "../headers/PauseMenu.h"
#include "../headers/GameOverMenu.h"

enum gameStates
{
    main_menu = 1,
    combat = 2,
    defeat = 3,
    victory = 4
};

class Game
{
    Game();
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    Settings &settings = Settings::getInstance();
    PauseMenu &pauseMenu = PauseMenu::getInstance();
    GameOverMenu &gameOverMenu = GameOverMenu::getInstance();

    bool openSettings = false;
    bool openPauseMenu = false;
    bool fullscreen = true;
    bool controls = true; // true for WASD, false for mouse
    int currentFramerate = 144;
    sf::Clock updateClock;
    gameStates currentState;

    mainMenu &menu = mainMenu::getInstance();

    std::unique_ptr<Player> player;
    std::unique_ptr<GUI> gui;
    std::unique_ptr<Level> level;

    sf::RenderWindow window;

    void setup();
    static void selectGameState(gameStates &);

    bool handleInputs();

    void draw();

    void handleNewState();

    void togglePause();
    void resetGame();

public:
    void Play();
    static Game &getInstance();
    ~Game() = default;

    friend std::ostream &operator<<(std::ostream &, const Game &);
};