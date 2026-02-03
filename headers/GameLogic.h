#pragma once

#include "../headers/MainMenu.h"
#include "../headers/Level.h"
#include "../headers/Settings.h"
#include "../headers/PauseMenu.h"
#include "../headers/GameOverMenu.h"
#include "../headers/Inventory.h"

enum gameStates
{
    main_menu = 1,
    combat = 2,
    defeat = 3,
    victory = 4
};

class Game : public Singleton<Game>
{
    Settings &settings = Settings::getInstance();
    PauseMenu &pauseMenu = PauseMenu::getInstance();
    GameOverMenu &gameOverMenu = GameOverMenu::getInstance();
    Inventory &inventory = Inventory::getInstance();

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

    bool handleInputs();

    void draw();

    void handleNewState();

    void togglePause();
    void resetGame();

protected:
    Game();

public:
    void Play();

    friend std::ostream &operator<<(std::ostream &, const Game &);
};