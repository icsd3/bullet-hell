#include "../headers/ResourceManager.h"
#include <unordered_set>

sf::Texture ResourceManager::playerTexture;
sf::Texture ResourceManager::playerProjectileTexture;
sf::Texture ResourceManager::enemyTexture;
sf::Texture ResourceManager::enemyProjectileTexture;
sf::Texture ResourceManager::bossTexture;
sf::Texture ResourceManager::bossProjectileTexture;
sf::Texture ResourceManager::baseRoomTexture;
sf::Texture ResourceManager::doorVerticalTexture;
sf::Texture ResourceManager::doorHorizontalTexture;
sf::Texture ResourceManager::obstacleTexture;
sf::Texture ResourceManager::augmentBackgroundTexture;
sf::Texture ResourceManager::augmentButtonTexture;
sf::Texture ResourceManager::menuBackgroundTexture;
sf::Texture ResourceManager::menuStartButtonTexture;
sf::Texture ResourceManager::menuSettingsButtonTexture;
sf::Texture ResourceManager::menuExitButtonTexture;

sf::Font ResourceManager::arial;
sf::Font ResourceManager::courier;

void ResourceManager::loadTexture(TextureType type)
{
    static std::unordered_set<TextureType> loadedTextures;
    if (loadedTextures.contains(type)) return;

    using enum TextureType;
    switch (type)
    {
        case Player:
            if(!playerTexture.loadFromFile("textures/player.png"))
                throw TextureLoadException("Failed to load player texture");
            break;
        case PlayerProjectile:
            if(!playerProjectileTexture.loadFromFile("textures/player_projectile.png"))
                throw TextureLoadException("Failed to load player projectile texture");
            break;
        case Enemy:
            if(!enemyTexture.loadFromFile("textures/enemy.png"))
                throw TextureLoadException("Failed to load enemy texture");
            break;
        case EnemyProjectile:
            if(!enemyProjectileTexture.loadFromFile("textures/enemy_projectile.png"))
                throw TextureLoadException("Failed to load enemy projectile texture");
            break;
        case Boss:
            if(!bossTexture.loadFromFile("textures/boss.png"))
                throw TextureLoadException("Failed to load boss texture");
            break;
        case BossProjectile:
            if(!bossProjectileTexture.loadFromFile("textures/boss_projectile.png"))
                throw TextureLoadException("Failed to load boss projectile texture");
            break;
        case BaseRoom:
            if(!baseRoomTexture.loadFromFile("textures/room_background.png"))
                throw TextureLoadException("Failed to load room background texture");
            break;
        case DoorVertical:
            if(!doorVerticalTexture.loadFromFile("textures/door_vertical.png"))
                throw TextureLoadException("Failed to load door vertical texture");
            break;
        case DoorHorizontal:
            if(!doorHorizontalTexture.loadFromFile("textures/door_horizontal.png"))
                throw TextureLoadException("Failed to load door horizontal texture");
            break;
        case Obstacle:
            if(!obstacleTexture.loadFromFile("textures/obstacle.png"))
                throw TextureLoadException("Failed to load obstacle texture");
            break;
        case AugmentBackground:
            if(!augmentBackgroundTexture.loadFromFile("textures/augment_background.png"))
                throw TextureLoadException("Failed to load augment background texture");
            break;
        case AugmentButton:
            if(!augmentButtonTexture.loadFromFile("textures/augment_button.png"))
                throw TextureLoadException("Failed to load augment button texture");
            break;
        case MenuBackground:
            if(!menuBackgroundTexture.loadFromFile("textures/menu_background.png"))
                throw TextureLoadException("Failed to load menu background texture");
            break;
        case MenuStartButton:
            if(!menuStartButtonTexture.loadFromFile("textures/start_button.png"))
                throw TextureLoadException("Failed to load menu start button texture");
            break;
        case MenuSettingsButton:
            if(!menuSettingsButtonTexture.loadFromFile("textures/settings_button.png"))
                throw TextureLoadException("Failed to load menu settings button texture");
            break;
        case MenuExitButton:
            if(!menuExitButtonTexture.loadFromFile("textures/exit_button.png"))
                throw TextureLoadException("Failed to load menu exit button texture");
            break;
        default:
            throw TextureLoadException("Invalid TextureType");
    }

    loadedTextures.insert(type);
}

sf::Texture &ResourceManager::getTexture(TextureType type)
{
    loadTexture(type);
    using enum TextureType;
    switch (type)
    {
        case Player:
            return playerTexture;
        case PlayerProjectile:
            return playerProjectileTexture;
        case Enemy:
            return enemyTexture;
        case EnemyProjectile:
            return enemyProjectileTexture;
        case Boss:
            return bossTexture;
        case BossProjectile:
            return bossProjectileTexture;
        case BaseRoom:
            return baseRoomTexture;
        case DoorVertical:
            return doorVerticalTexture;
        case DoorHorizontal:
            return doorHorizontalTexture;
        case Obstacle:
            return obstacleTexture;
        case AugmentBackground:
            return augmentBackgroundTexture;
        case AugmentButton:
            return augmentButtonTexture;
        case MenuBackground:
            return menuBackgroundTexture;
        case MenuStartButton:
            return menuStartButtonTexture;
        case MenuSettingsButton:
            return menuSettingsButtonTexture;
        case MenuExitButton:
            return menuExitButtonTexture;
        default:
            throw TextureFetchException("Invalid TextureType");
    }
}

void ResourceManager::loadFont(FontType type)
{
    static std::unordered_set<FontType> loadedFonts;
    if (loadedFonts.contains(type)) return;

    using enum FontType;
    switch (type)
    {
        case Arial:
            if(!arial.openFromFile("fonts/arial.ttf"))
                throw FontLoadException("Failed to load Arial font");
            break;
        case Courier:
            if(!courier.openFromFile("fonts/courier.ttf"))
                throw FontLoadException("Failed to load Courier font");
            break;
        default:
            throw FontLoadException("Invalid FontType");
    }

    loadedFonts.insert(type);
}

sf::Font &ResourceManager::getFont(FontType type)
{
    loadFont(type);
    using enum FontType;
    switch (type)
    {
        case Arial:
            return arial;
        case Courier:
            return courier;
        default:
            throw FontFetchException("Invalid FontType");
    }
}