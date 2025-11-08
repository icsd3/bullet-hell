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

    std::string menuBackgroundPath = "textures/menu_background.png";
    std::string menuButtonPath[2] = {
        "textures/start_button.png",
        "textures/exit_button.png"
    };
    std::string augmentBackgroundPath = "textures/augment_background.png";
    std::string augmentButtonPath = "textures/augment_button.png";
    std::string levelBackgroundPath = "textures/level_background.png";
    std::string enemyPath = "textures/enemy.png";
    std::string playerProjectilePath = "textures/player_projectile.png";
    std::string enemyProjectilePath = "textures/enemy_projectile.png";

    sf::Texture menuBackgroundTexture;
    std::optional<sf::Sprite> menuBackgroundSprite;
    sf::Texture menuButtonTexture[2];
    std::optional<sf::Sprite> menuButtonSprite[2];

    sf::Texture augmentBackgroundTexture;
    std::optional<sf::Sprite> augmentBackgroundSprite;
    sf::Texture augmentButtonTexture[3];
    std::optional<sf::Sprite> augmentButtonSprite[3];

    sf::Texture levelBackgroundTexture;
    std::optional<sf::Sprite> levelBackgroundSprite;

    sf::Texture playerTexture;
    sf::Texture enemyTexture;
    sf::Texture playerProjectileTexture;
    sf::Texture enemyProjectileTexture;

public:
    static AssetLoader& getInstance();
    ~AssetLoader() = default;
    friend std::ostream& operator<<(std::ostream& os, const AssetLoader& loader);

    void loadStaticAssets();
    void loadMainMenu(sf::RenderWindow&);
    void loadAugment(sf::RenderWindow&);
    void loadLevel(sf::RenderWindow&);
    // void loadDefeat(sf::RenderWindow&);
    // void loadVictory(sf::RenderWindow&);

    bool hasMenuBackgroundSprite();
    bool hasAugmentBackgroundSprite();
    bool hasLevelBackgroundSprite();
    bool hasMenuButtonSprites();
    bool hasAugmentButtonSprites();

    sf::Texture& getPlayerTexture();
    sf::Texture& getEnemyTexture();
    sf::Texture& getPlayerProjectileTexture();
    sf::Texture& getEnemyProjectileTexture();

    sf::FloatRect getMenuButtonBounds(int);
    sf::FloatRect getAugmentButtonBounds(int);

    void drawMenu(sf::RenderWindow&);
    void drawAugment(sf::RenderWindow&);
    void drawLevelBackground(sf::RenderWindow&);
};