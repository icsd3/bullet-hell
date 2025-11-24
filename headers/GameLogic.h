#pragma once

#include "../headers/MainMenu.h"
#include "../headers/Level.h"
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

    bool paused = false;
    bool openSettings = false;
    bool fullscreen = true;
    bool controls = true; //true for WASD, false for mouse
    sf::Clock updateClock;
    sf::Vector2f target;
    gameStates currentState;

    mainMenu &menu = mainMenu::getInstance();
    Augment &augment = Augment::getInstance();
    Level &level = Level::getInstance();
    Player &player = Player::getInstance();
    GUI &gui = GUI::getInstance();

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