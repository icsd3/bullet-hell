#include "../headers/ResourceManager.h"

sf::Texture ResourceManager::playerTexture;
sf::Texture ResourceManager::playerProjectileTexture;
sf::Texture ResourceManager::playerSlashTexture;
sf::Texture ResourceManager::playerStabTexture;
sf::Texture ResourceManager::playerBeamTexture;

sf::Texture ResourceManager::enemyTexture;
sf::Texture ResourceManager::enemyProjectileTexture;
sf::Texture ResourceManager::enemySlashTexture;
sf::Texture ResourceManager::enemyStabTexture;
sf::Texture ResourceManager::enemyBeamTexture;

sf::Texture ResourceManager::bossTexture;
sf::Texture ResourceManager::bossProjectileTexture;
sf::Texture ResourceManager::bossSlashTexture;
sf::Texture ResourceManager::bossStabTexture;
sf::Texture ResourceManager::bossBeamTexture;

sf::Texture ResourceManager::baseRoomTexture;
sf::Texture ResourceManager::doorVerticalTexture;
sf::Texture ResourceManager::doorHorizontalTexture;
sf::Texture ResourceManager::obstacleTexture;

sf::Texture ResourceManager::menuBackgroundTexture;
sf::Texture ResourceManager::menuStartButtonTexture;
sf::Texture ResourceManager::menuSettingsButtonTexture;
sf::Texture ResourceManager::menuExitButtonTexture;

sf::Texture ResourceManager::pauseResumeButtonTexture;
sf::Texture ResourceManager::pauseMenuButtonTexture;

sf::Texture ResourceManager::pistolTexture;
sf::Texture ResourceManager::shotgunTexture;
sf::Texture ResourceManager::rifleTexture;
sf::Texture ResourceManager::sniperTexture;
sf::Texture ResourceManager::swordTexture;
sf::Texture ResourceManager::spearTexture;
sf::Texture ResourceManager::laserTexture;

sf::Texture ResourceManager::heartTexture;
sf::Texture ResourceManager::weaponTexture;

sf::Font ResourceManager::arial;
sf::Font ResourceManager::courier;

void ResourceManager::loadTexture(TextureType type)
{
    static std::unordered_set<TextureType> loadedTextures;
    if (loadedTextures.contains(type)) 
        return;

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
        case PlayerSlash:
            if(!playerSlashTexture.loadFromFile("textures/player_slash.png"))
                throw TextureLoadException("Failed to load player slash texture");
            break;
        case PlayerStab:
            if(!playerStabTexture.loadFromFile("textures/player_stab.png"))
                throw TextureLoadException("Failed to load player stab texture");
            break;
        case PlayerBeam:
            if(!playerBeamTexture.loadFromFile("textures/player_beam.png"))
                throw TextureLoadException("Failed to load player beam texture");
            break;
        case Enemy:
            if(!enemyTexture.loadFromFile("textures/enemy.png"))
                throw TextureLoadException("Failed to load enemy texture");
            break;
        case EnemyProjectile:
            if(!enemyProjectileTexture.loadFromFile("textures/enemy_projectile.png"))
                throw TextureLoadException("Failed to load enemy projectile texture");
            break;
        case EnemySlash:
            if(!enemySlashTexture.loadFromFile("textures/enemy_slash.png"))
                throw TextureLoadException("Failed to load enemy slash texture");
            break;
        case EnemyStab:
            if(!enemyStabTexture.loadFromFile("textures/enemy_stab.png"))
                throw TextureLoadException("Failed to load enemy stab texture");
            break;
        case EnemyBeam:
            if(!enemyBeamTexture.loadFromFile("textures/enemy_beam.png"))
                throw TextureLoadException("Failed to load enemy beam texture");
            break;
        case Boss:
            if(!bossTexture.loadFromFile("textures/boss.png"))
                throw TextureLoadException("Failed to load boss texture");
            break;
        case BossProjectile:
            if(!bossProjectileTexture.loadFromFile("textures/boss_projectile.png"))
                throw TextureLoadException("Failed to load boss projectile texture");
            break;
        case BossSlash:
            if(!bossSlashTexture.loadFromFile("textures/boss_slash.png"))
                throw TextureLoadException("Failed to load boss slash texture");
            break;
        case BossStab:
            if(!bossStabTexture.loadFromFile("textures/boss_stab.png"))
                throw TextureLoadException("Failed to load boss stab texture");
            break;
        case BossBeam:
            if(!bossBeamTexture.loadFromFile("textures/boss_beam.png"))
                throw TextureLoadException("Failed to load boss beam texture");
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
        case PauseResumeButton:
            if(!pauseResumeButtonTexture.loadFromFile("textures/resume_button.png"))
                throw TextureLoadException("Failed to load pause resume button texture");
            break;
        case PauseMenuButton:
            if(!pauseMenuButtonTexture.loadFromFile("textures/menu_button.png"))
                throw TextureLoadException("Failed to load pause menu button texture");
            break;
        case Pistol:
            if(!pistolTexture.loadFromFile("textures/pistol.png"))
                throw TextureLoadException("Failed to load pistol texture");
            break;
        case Shotgun:
            if(!shotgunTexture.loadFromFile("textures/shotgun.png"))
                throw TextureLoadException("Failed to load shotgun texture");
            break;
        case Rifle:
            if(!rifleTexture.loadFromFile("textures/rifle.png"))
                throw TextureLoadException("Failed to load rifle texture");
            break;
        case Sniper:
            if(!sniperTexture.loadFromFile("textures/sniper.png"))
                throw TextureLoadException("Failed to load sniper texture");
            break;
        case Sword:
            if(!swordTexture.loadFromFile("textures/sword.png"))
                throw TextureLoadException("Failed to load sword texture");
            break;
        case Spear:
            if(!spearTexture.loadFromFile("textures/spear.png"))
                throw TextureLoadException("Failed to load spear texture");
            break;
        case Laser:
            if(!laserTexture.loadFromFile("textures/laser.png"))
                throw TextureLoadException("Failed to load laser texture");
            break;
        case Heart:
            if(!heartTexture.loadFromFile("textures/heart.png"))
                throw TextureLoadException("Failed to load heart texture");
            break;
        case Weapon:
            if(!weaponTexture.loadFromFile("textures/weapon.png"))
                throw TextureLoadException("Failed to load weapon texture");
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
        case PlayerSlash:
            return playerSlashTexture;
        case PlayerStab:
            return playerStabTexture;
        case PlayerBeam:
            return playerBeamTexture;
        case Enemy:
            return enemyTexture;
        case EnemyProjectile:
            return enemyProjectileTexture;
        case EnemySlash:
            return enemySlashTexture;
        case EnemyStab:
            return enemyStabTexture;
        case EnemyBeam:
            return enemyBeamTexture;
        case Boss:
            return bossTexture;
        case BossProjectile:
            return bossProjectileTexture;
        case BossSlash:
            return bossSlashTexture;
        case BossStab:
            return bossStabTexture;
        case BossBeam:
            return bossBeamTexture;
        case BaseRoom:
            return baseRoomTexture;
        case DoorVertical:
            return doorVerticalTexture;
        case DoorHorizontal:
            return doorHorizontalTexture;
        case Obstacle:
            return obstacleTexture;
        case MenuBackground:
            return menuBackgroundTexture;
        case MenuStartButton:
            return menuStartButtonTexture;
        case MenuSettingsButton:
            return menuSettingsButtonTexture;
        case MenuExitButton:
            return menuExitButtonTexture;
        case PauseResumeButton:
            return pauseResumeButtonTexture;
        case PauseMenuButton:
            return pauseMenuButtonTexture;
        case Pistol:
            return pistolTexture;
        case Shotgun:
            return shotgunTexture;
        case Rifle:
            return rifleTexture;
        case Sniper:
            return sniperTexture;
        case Sword:
            return swordTexture;
        case Spear:
            return spearTexture;
        case Laser:
            return laserTexture;
        case Heart:
            return heartTexture;
        case Weapon:
            return weaponTexture;
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