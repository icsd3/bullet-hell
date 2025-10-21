#pragma once

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
    sf::Texture m_backgroundTexture;
    std::optional<sf::Sprite> m_backgroundSprite;

    void loadBackground();
    void setUp();

public:
    void Play();
    static Game& getInstance();

    friend std::ostream& operator<<(std::ostream& os, const Game& game);

    ~Game() = default;
};
