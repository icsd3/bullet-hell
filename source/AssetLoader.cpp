#include "../headers/AssetLoader.h"
#include "../headers/Utils.h"
#include <iostream>

AssetLoader::AssetLoader()
{
}

AssetLoader &AssetLoader::getInstance()
{
    static AssetLoader instance;
    return instance;
}

std::ostream &operator<<(std::ostream &os, const AssetLoader &loader)
{
    os << "Loader:\n";
    os << "    Level Background Loaded: " << (loader.levelBackgroundSprite.has_value() ? loader.levelBackgroundPath : "Not Loaded");
    os << "\n";
    return os;
}

void AssetLoader::loadStaticAssets()
{
    if (!levelBackgroundTexture.loadFromFile(levelBackgroundPath))
    {
        std::cerr << "Error loading level_background.png\n";
    }
    levelBackgroundSprite.emplace(levelBackgroundTexture);

    if (!enemyTexture.loadFromFile(enemyPath))
    {
        std::cerr << "Error loading enemy.png\n";
    }

    if (!playerProjectileTexture.loadFromFile(playerProjectilePath))
    {
        std::cerr << "Error loading player_projectile.png\n";
    }

    if (!enemyProjectileTexture.loadFromFile(enemyProjectilePath))
    {
        std::cerr << "Error loading enemy_projectile.png\n";
    }
}

void AssetLoader::loadLevel()
{
    if (levelBackgroundSprite)
    {
        const sf::Vector2u textureSize = levelBackgroundTexture.getSize();
        levelBackgroundSprite->setScale(sf::Vector2f(
            1.f * LOGICAL_WIDTH / textureSize.x,
            1.f * LOGICAL_HEIGHT / textureSize.y));
    }
}

// void AssetLoader::loadDefeat(sf::RenderWindow& window)
// {
//
// }

// void AssetLoader::loadVictory(sf::RenderWindow& window)
// {
//
// }

bool AssetLoader::hasLevelBackgroundSprite()
{
    return levelBackgroundSprite.has_value();
}

sf::Texture &AssetLoader::getPlayerTexture()
{
    if (playerTexture.getSize().x == 0 || playerTexture.getSize().y == 0)
    {
        if (!playerTexture.loadFromFile("textures/player.png"))
        {
            std::cerr << "Error loading player.png\n";
        }
    }
    return playerTexture;
}

sf::Texture &AssetLoader::getEnemyTexture()
{
    return enemyTexture;
}

sf::Texture &AssetLoader::getPlayerProjectileTexture()
{
    return playerProjectileTexture;
}

void AssetLoader::drawLevelBackground(sf::RenderWindow &window)
{
    if (hasLevelBackgroundSprite()) {
        sf::Vector2f scaleFactor = getScaleFactor(window);
        sf::Sprite drawBg = *levelBackgroundSprite;
        drawBg.setPosition(mapToScreen(levelBackgroundSprite->getPosition(), window));
        drawBg.scale(scaleFactor);
        window.draw(drawBg);
    }
}