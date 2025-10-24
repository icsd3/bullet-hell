#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <optional>

class Game;

class AssetLoader
{
    AssetLoader();
    AssetLoader(const AssetLoader&) = delete;
    AssetLoader& operator=(const AssetLoader&) = delete;

    friend class Game;

    sf::Texture menuBackgroundTexture;
    std::optional<sf::Sprite> menuBackgroundSprite;
    sf::Texture menuButtonTexture[2];
    std::optional<sf::Sprite> menuButtonSprite[2];

    sf::Texture augmentBackgroundTexture;
    std::optional<sf::Sprite> augmentBackgroundSprite;
    sf::Texture augmentButtonTexture[3];
    std::optional<sf::Sprite> augmentButtonSprite[3];

    void loadMainMenu(sf::RenderWindow&);
    void loadAugment(sf::RenderWindow&);
    void loadLevel(sf::RenderWindow&);
    void loadDefeat(sf::RenderWindow&);
    void loadVictory(sf::RenderWindow&);

public:
    static AssetLoader& getInstance();
    ~AssetLoader() = default;
};