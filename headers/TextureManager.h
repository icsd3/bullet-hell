#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class TextureManager
{
    static sf::Texture playerTexture;
    static sf::Texture playerProjectileTexture;
    static sf::Texture enemyTexture;
    static sf::Texture enemyProjectileTexture;
    // static sf::Texture bossTexture;
    // static sf::Texture bossProjectileTexture;
    static sf::Texture baseRoomTexture;
    static sf::Texture doorVerticalTexture;
    static sf::Texture doorHorizontalTexture;
    static sf::Texture obstacleTexture;
    static sf::Texture augmentBackgroundTexture;
    static sf::Texture augmentButtonTexture;
    static sf::Texture menuBackgroundTexture;
    static sf::Texture menuStartButtonTexture;
    static sf::Texture menuSettingsButtonTexture;
    static sf::Texture menuExitButtonTexture;

public:
    static sf::Texture &getPlayerTexture();
    static sf::Texture &getPlayerProjectileTexture();
    static sf::Texture &getEnemyTexture();
    static sf::Texture &getEnemyProjectileTexture();
    static sf::Texture &getBossTexture();
    static sf::Texture &getBossProjectileTexture();
    static sf::Texture &getBaseRoomTexture();
    static sf::Texture &getDoorVerticalTexture();
    static sf::Texture &getDoorHorizontalTexture();
    static sf::Texture &getObstacleTexture();
    static sf::Texture &getAugmentBackgroundTexture();
    static sf::Texture &getAugmentButtonTexture();
    static sf::Texture &getMenuBackgroundTexture();
    static sf::Texture &getMenuStartButtonTexture();
    static sf::Texture &getMenuSettingsButtonTexture();
    static sf::Texture &getMenuExitButtonTexture();
};