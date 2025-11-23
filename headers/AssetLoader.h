#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

class Game;

class AssetLoader
{
    AssetLoader();
    AssetLoader(const AssetLoader &) = delete;
    AssetLoader &operator=(const AssetLoader &) = delete;

    std::string levelBackgroundPath = "textures/level_background.png";
    std::string enemyPath = "textures/enemy.png";
    std::string playerProjectilePath = "textures/player_projectile.png";
    std::string enemyProjectilePath = "textures/enemy_projectile.png";

    sf::Texture levelBackgroundTexture;
    std::optional<sf::Sprite> levelBackgroundSprite;

    sf::Texture playerTexture;
    sf::Texture enemyTexture;
    sf::Texture playerProjectileTexture;
    sf::Texture enemyProjectileTexture;

public:
    static AssetLoader &getInstance();
    ~AssetLoader() = default;
    friend std::ostream &operator<<(std::ostream &, const AssetLoader &);

    void loadStaticAssets();
    void loadLevel();
    // void loadDefeat(sf::RenderWindow&);
    // void loadVictory(sf::RenderWindow&);

    bool hasLevelBackgroundSprite();

    sf::Texture &getPlayerTexture();
    sf::Texture &getEnemyTexture();
    sf::Texture &getPlayerProjectileTexture();

    void drawLevelBackground(sf::RenderWindow &);
};