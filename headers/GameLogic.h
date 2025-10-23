#pragma once

#include "../headers/GameStates.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <optional>

class Game
{
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    sf::RenderWindow window;

    sf::Texture menuBackgroundTexture;
    std::optional<sf::Sprite> menuBackgroundSprite;
    sf::Texture menuButtonTexture[2];
    std::optional<sf::Sprite> menuButtonSprite[2];

    sf::Texture augmentBackgroundTexture;
    std::optional<sf::Sprite> augmentBackgroundSprite;
    sf::Texture augmentButtonTexture[3];
    std::optional<sf::Sprite> augmentButtonSprite[3];

    gameStates currentState;

    void setup();

    void loadMainMenu();
    void loadAugment();
    void loadLevel();
    void loadDefeat();
    void loadVictory();

    bool handleInputs();
    bool handleMainMenuInput(const sf::Event& event);

    void draw();
    void drawMenu();
    void drawAugment();
    void drawLevel();
    void drawDefeat();
    void drawVictory();
    
    void handleNewState(gameStates newState);

public:
    void Play();
    static Game& getInstance();
    ~Game() = default;
};
