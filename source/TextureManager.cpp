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

sf::Texture &TextureManager::getPlayerTexture()
{
    return playerTexture;
}

sf::Texture &TextureManager::getPlayerProjectileTexture()
{
    return playerProjectileTexture;
}

sf::Texture &TextureManager::getEnemyTexture()
{
    return enemyTexture;
}

sf::Texture &TextureManager::getEnemyProjectileTexture()
{
    return enemyProjectileTexture;
}

sf::Texture &TextureManager::getBossTexture()
{
    return bossTexture;
}

sf::Texture &TextureManager::getBossProjectileTexture()
{
    return bossProjectileTexture;
}

sf::Texture &TextureManager::getBaseRoomTexture()
{
    return baseRoomTexture;
}

sf::Texture &TextureManager::getDoorVerticalTexture()
{
    return doorVerticalTexture;
}

sf::Texture &TextureManager::getDoorHorizontalTexture()
{
    return doorHorizontalTexture;
}

sf::Texture &TextureManager::getObstacleTexture()
{
    return obstacleTexture;
}

sf::Texture &TextureManager::getAugmentBackgroundTexture()
{
    return augmentBackgroundTexture;
}

sf::Texture &TextureManager::getAugmentButtonTexture()
{
    return augmentButtonTexture;
}

sf::Texture &TextureManager::getMenuBackgroundTexture()
{
    return menuBackgroundTexture;
}

sf::Texture &TextureManager::getMenuStartButtonTexture()
{
    return menuStartButtonTexture;
}

sf::Texture &TextureManager::getMenuSettingsButtonTexture()
{
    return menuSettingsButtonTexture;
}

sf::Texture &TextureManager::getMenuExitButtonTexture()
{
    return menuExitButtonTexture;
}
