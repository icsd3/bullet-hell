#pragma once

#include "../headers/GameStates.h"
#include "../headers/AssetLoader.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <optional>

class Game
{
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    AssetLoader& Loader = AssetLoader::getInstance();

    sf::RenderWindow window;

    gameStates currentState;
    
    void setup();

    bool handleInputs();
    bool handleMainMenuInput(const sf::Event& event);
    void handleAugmentInput(const sf::Event& event);

    void draw();
    void drawMenu();
    void drawAugment();
    void drawLevel();
    void drawDefeat();
    void drawVictory();
    
    void handleNewState();

public:
    void Play();
    static Game& getInstance();
    ~Game() = default;
};