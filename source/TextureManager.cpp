#include "../headers/TextureManager.h"

sf::Texture TextureManager::playerTexture("textures/player.png");
sf::Texture TextureManager::playerProjectileTexture("textures/player_projectile.png");
sf::Texture TextureManager::enemyTexture("textures/enemy.png");
sf::Texture TextureManager::enemyProjectileTexture("textures/enemy_projectile.png");
sf::Texture TextureManager::bossTexture("textures/boss.png");
sf::Texture TextureManager::bossProjectileTexture("textures/boss_projectile.png");
sf::Texture TextureManager::baseRoomTexture("textures/room_background.png");
sf::Texture TextureManager::doorVerticalTexture("textures/door_vertical.png");
sf::Texture TextureManager::doorHorizontalTexture("textures/door_horizontal.png");
sf::Texture TextureManager::obstacleTexture("textures/obstacle.png");
sf::Texture TextureManager::augmentBackgroundTexture("textures/augment_background.png");
sf::Texture TextureManager::augmentButtonTexture("textures/augment_button.png");
sf::Texture TextureManager::menuBackgroundTexture("textures/menu_background.png");
sf::Texture TextureManager::menuStartButtonTexture("textures/start_button.png");
sf::Texture TextureManager::menuSettingsButtonTexture("textures/settings_button.png");
sf::Texture TextureManager::menuExitButtonTexture("textures/exit_button.png");

sf::Texture &TextureManager::getTexture(TextureType type)
{
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
            throw std::invalid_argument("Invalid TextureType");
    }
}