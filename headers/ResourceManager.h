#pragma once

#include "../headers/Exception.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_set>

enum class TextureType
{
    Player,
    PlayerProjectile,
    PlayerSlash,
    PlayerStab,
    PlayerBeam,
    Enemy,
    EnemyProjectile,
    EnemySlash,
    EnemyStab,
    EnemyBeam,
    Boss,
    BossProjectile,
    BossSlash,
    BossStab,
    BossBeam,
    BaseRoom,
    DoorVertical,
    DoorHorizontal,
    Obstacle,
    MenuBackground,
    MenuStartButton,
    MenuSettingsButton,
    MenuExitButton,
    PauseResumeButton,
    PauseMenuButton,
    Pistol,
    Shotgun,
    Rifle,
    Sniper,
    Sword,
    Spear,
    Laser,
    Heart,
    Weapon
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
    static sf::Texture playerSlashTexture;
    static sf::Texture playerStabTexture;
    static sf::Texture playerBeamTexture;

    static sf::Texture enemyTexture;
    static sf::Texture enemyProjectileTexture;
    static sf::Texture enemySlashTexture;
    static sf::Texture enemyStabTexture;
    static sf::Texture enemyBeamTexture;

    static sf::Texture bossTexture;
    static sf::Texture bossProjectileTexture;
    static sf::Texture bossSlashTexture;
    static sf::Texture bossStabTexture;
    static sf::Texture bossBeamTexture;

    static sf::Texture baseRoomTexture;
    static sf::Texture doorVerticalTexture;
    static sf::Texture doorHorizontalTexture;
    static sf::Texture obstacleTexture;

    static sf::Texture menuBackgroundTexture;
    static sf::Texture menuStartButtonTexture;
    static sf::Texture menuSettingsButtonTexture;
    static sf::Texture menuExitButtonTexture;

    static sf::Texture pauseResumeButtonTexture;
    static sf::Texture pauseMenuButtonTexture;

    static sf::Texture pistolTexture;
    static sf::Texture shotgunTexture;
    static sf::Texture rifleTexture;
    static sf::Texture sniperTexture;
    static sf::Texture swordTexture;
    static sf::Texture spearTexture;
    static sf::Texture laserTexture;

    static sf::Texture heartTexture;
    static sf::Texture weaponTexture;

    static sf::Font arial;
    static sf::Font courier;

public:
    static void loadTexture(TextureType type);
    static sf::Texture &getTexture(TextureType type);

    static void loadFont(FontType type);
    static sf::Font &getFont(FontType type);
};