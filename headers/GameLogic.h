#pragma once

#include "../headers/GameStates.h"
#include "../headers/AssetLoader.h"
#include "../headers/Player.h"
#include "../headers/GUI.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <optional>

class Game
{
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    AssetLoader& loader = AssetLoader::getInstance();
    Player& player = Player::getInstance();
    // GUI& gui = GUI::getInstance();

    sf::RenderWindow window;
    sf::Clock clock;
    sf::Vector2f target;

    gameStates currentState;
    
    void setup();

    bool handleInputs();
    bool handleMainMenuInput(const sf::Event& event);
    void handleAugmentInput(const sf::Event& event);
    void handleLevelInput(const sf::Event& event);

    void draw();
    void drawMenu();
    void drawAugment();
    void drawLevel();
    // void drawGUI();
    // void drawDefeat();
    // void drawVictory();
    
    void handleNewState();

    friend void Player::updatePlayer(sf::Clock&, const sf::Vector2f&);

public:
    void Play();
    static Game& getInstance();
    ~Game() = default;

    friend std::ostream& operator<<(std::ostream& os, const Game& game);
};