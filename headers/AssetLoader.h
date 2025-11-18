#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

class Game;

class AssetLoader
{
    AssetLoader();
    AssetLoader(const AssetLoader &) = delete;
    AssetLoader &operator=(const AssetLoader &) = delete;

    std::string augmentBackgroundPath = "textures/augment_background.png";
    std::string augmentButtonPath = "textures/augment_button.png";
    std::string levelBackgroundPath = "textures/level_background.png";
    std::string enemyPath = "textures/enemy.png";
    std::string playerProjectilePath = "textures/player_projectile.png";
    std::string enemyProjectilePath = "textures/enemy_projectile.png";

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
    static AssetLoader &getInstance();
    ~AssetLoader() = default;
    friend std::ostream &operator<<(std::ostream &os, const AssetLoader &loader);

    void loadStaticAssets();
    void loadAugment(sf::RenderWindow &);
    void loadLevel(sf::RenderWindow &);
    // void loadDefeat(sf::RenderWindow&);
    // void loadVictory(sf::RenderWindow&);

    bool hasAugmentBackgroundSprite();
    bool hasLevelBackgroundSprite();
    bool hasAugmentButtonSprites();

    sf::Texture &getPlayerTexture();
    sf::Texture &getEnemyTexture();
    sf::Texture &getPlayerProjectileTexture();
    sf::FloatRect getAugmentButtonBounds(int);

    void drawAugment(sf::RenderWindow &);
    void drawLevelBackground(sf::RenderWindow &);
};