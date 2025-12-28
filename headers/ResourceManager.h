#pragma once

#include "../headers/Exception.h"

#include <SFML/Graphics.hpp>
#include <iostream>

enum class TextureType
{
    Player,
    PlayerProjectile,
    Enemy,
    EnemyProjectile,
    Boss,
    BossProjectile,
    BaseRoom,
    DoorVertical,
    DoorHorizontal,
    Obstacle,
    AugmentBackground,
    AugmentButton,
    MenuBackground,
    MenuStartButton,
    MenuSettingsButton,
    MenuExitButton,
    Pistol,
    Shotgun,
    Rifle,
    Sniper
};

enum class FontType
{
    Arial,
    Courier
};

class ResourceManager
{
    static sf::Texture playerTexture;
    static sf::Texture playerProjectileTexture;

    static sf::Texture enemyTexture;
    static sf::Texture enemyProjectileTexture;
    static sf::Texture bossTexture;
    static sf::Texture bossProjectileTexture;

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

    static sf::Texture pistolTexture;
    static sf::Texture shotgunTexture;
    static sf::Texture rifleTexture;
    static sf::Texture sniperTexture;

    static sf::Font arial;
    static sf::Font courier;

public:
    static void loadTexture(TextureType type);
    static sf::Texture &getTexture(TextureType type);

    static void loadFont(FontType type);
    static sf::Font &getFont(FontType type);
};