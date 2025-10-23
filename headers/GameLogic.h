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

    sf::RenderWindow m_window;

    sf::Texture m_menuBackgroundTexture;
    std::optional<sf::Sprite> m_menuBackgroundSprite;

    sf::Texture m_startButtonTexture;
    std::optional<sf::Sprite> m_startButtonSprite;

    sf::Texture m_augmentBackgroundTexture;
    std::optional<sf::Sprite> m_augmentBackgroundSprite;

    sf::Texture m_augmentButtonTexture[3];
    std::optional<sf::Sprite> m_augmentButtonSprite[3];

    gameStates m_currentState;

    void setup();

    void loadMainMenu();
    void loadAugment();
    void loadLevel();

    bool handleInputs();
    void handleMainMenuInput(const sf::Event& event);

    void draw();
    void drawMenu();
    void drawAugment();
    void drawLevel();
    void drawVictory();
    
    void handleNewState(gameStates newState);

public:
    void Play();
    static Game& getInstance();
    ~Game() = default;
};
