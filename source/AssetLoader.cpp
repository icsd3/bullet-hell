#include "../headers/AssetLoader.h"
#include <iostream>

AssetLoader::AssetLoader()
{

}

AssetLoader& AssetLoader::getInstance()
{
    static AssetLoader instance;
    return instance;
}

std::ostream& operator<<(std::ostream& os, const AssetLoader& loader)
{
    os << "    Menu Background Loaded: " << (loader.menuBackgroundSprite.has_value() ? loader.menuBackgroundPath : "Not Loaded") << "\n";
    for(int i = 0; i<2; i++)
    {
        os << "    Menu Button " << i << " Loaded: " << (loader.menuButtonSprite[i].has_value() ? loader.menuButtonPath[i] : "Not Loaded") << "\n";
    }
    os << "    Augment Background Loaded: " << (loader.augmentBackgroundSprite.has_value() ? loader.augmentBackgroundPath : "Not Loaded") << "\n";
    for(int i = 0; i<3; i++)
    {
        os << "    Augment Button " << i << " Loaded: " << (loader.augmentButtonSprite[i].has_value() ? loader.augmentButtonPath : "Not Loaded") << "\n";
    }
    os << "    Level Background Loaded: " << (loader.levelBackgroundSprite.has_value() ? loader.levelBackgroundPath : "Not Loaded");
    return os;
}

void AssetLoader::loadStaticAssets()
{
    if (!menuBackgroundTexture.loadFromFile(menuBackgroundPath))
    {
        std::cerr << "Error loading menu_background.png\n";
    }
    menuBackgroundSprite.emplace(menuBackgroundTexture);

    for(int i = 0; i<2; i++)
    {
         if(i==0)
        {
            if (!menuButtonTexture[i].loadFromFile(menuButtonPath[0]))
            {
                std::cerr << "Error loading start_button.png\n";
            }
        }
        else
        {
            if (!menuButtonTexture[i].loadFromFile(menuButtonPath[1]))
            {
                std::cerr << "Error loading exit_button.png\n";
            }
        }
        menuButtonSprite[i].emplace(menuButtonTexture[i]);
    }

    if (!augmentBackgroundTexture.loadFromFile(augmentBackgroundPath))
    {
        std::cerr << "Error loading augment_background.png\n";
    }
    augmentBackgroundSprite.emplace(augmentBackgroundTexture);

    for(int i = 0; i<3; i++)
    {
        if (!augmentButtonTexture[i].loadFromFile(augmentButtonPath))
        {
            std::cerr << "Error loading augment_button.png\n";
        }
        augmentButtonSprite[i].emplace(augmentButtonTexture[i]);
    }

    if(!levelBackgroundTexture.loadFromFile(levelBackgroundPath))
    {
        std::cerr << "Error loading level_background.png\n";
    }
    levelBackgroundSprite.emplace(levelBackgroundTexture);
}

void AssetLoader::loadMainMenu(sf::RenderWindow& window)
{
    if(menuBackgroundSprite)
    {
        const sf::Vector2u windowSize = window.getSize();
        const sf::Vector2u textureSize = menuBackgroundTexture.getSize();
        menuBackgroundSprite->setScale(sf::Vector2f(
            static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x),
            static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y)
        ));
    }
    for(int i = 0; i<2; i++)
    {
        if(menuButtonSprite[i])
        {
            const sf::Vector2u windowSize = window.getSize();
            const sf::Vector2u startButtonSize = menuButtonTexture[i].getSize();
            const float scale = static_cast<float>(windowSize.x) / static_cast<float>(startButtonSize.x) /4.f;
            menuButtonSprite[i]->setScale(sf::Vector2f(scale, scale));
            menuButtonSprite[i]->setOrigin(sf::Vector2f(
                startButtonSize.x / 2.f,
                startButtonSize.y / 2.f
            ));
            menuButtonSprite[i]->setPosition(sf::Vector2f(
                windowSize.x / 2.f,
                windowSize.y / 3.f * (i+1)
            ));
        }
    }
    
}

void AssetLoader::loadAugment(sf::RenderWindow& window)
{ 
    if(augmentBackgroundSprite)
    {
        const sf::Vector2u windowSize = window.getSize();
        const sf::Vector2u textureSize = augmentBackgroundTexture.getSize();
        augmentBackgroundSprite->setScale(sf::Vector2f(
            static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x),
            static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y)
        ));
    }

    for(int i = 0; i<3; i++)
    {
        if(augmentButtonSprite[i])
        {
            const sf::Vector2u windowSize = window.getSize();
            const sf::Vector2u augmentButtonSize = augmentButtonTexture[i].getSize();
            const float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(augmentButtonSize.x) /13.f * 3;
            const float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(augmentButtonSize.y) / 10.f * 7.5f;
            augmentButtonSprite[i]->setScale(sf::Vector2f(scaleX, scaleY));
            augmentButtonSprite[i]->setOrigin(sf::Vector2f(
                augmentButtonSize.x / 2.f,
                augmentButtonSize.y / 2.f
            ));
            augmentButtonSprite[i]->setPosition(sf::Vector2f(
                windowSize.x / 26.f * (8*i+5),
                windowSize.y / 2.f
            ));
        }
    }
}

void AssetLoader::loadLevel(sf::RenderWindow& window)
{
    if(levelBackgroundSprite)
    {
        const sf::Vector2u windowSize = window.getSize();
        const sf::Vector2u textureSize = levelBackgroundTexture.getSize();
        levelBackgroundSprite->setScale(sf::Vector2f(
            static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x),
            static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y)
        ));
    }
}

// void AssetLoader::loadDefeat(sf::RenderWindow& window)
// {
//    
// }

// void AssetLoader::loadVictory(sf::RenderWindow& window)
// {
    
// }

bool AssetLoader::hasMenuBackgroundSprite()
{
    return menuBackgroundSprite.has_value();
}

bool AssetLoader::hasAugmentBackgroundSprite()
{
    return augmentBackgroundSprite.has_value();
}

bool AssetLoader::hasLevelBackgroundSprite()
{
    return levelBackgroundSprite.has_value();
}

bool AssetLoader::hasMenuButtonSprites()
{
    return (menuButtonSprite[0].has_value() && menuButtonSprite[1].has_value());
}

bool AssetLoader::hasAugmentButtonSprites()
{
    return (augmentButtonSprite[0].has_value() && augmentButtonSprite[1].has_value() && augmentButtonSprite[2].has_value());
}

sf::FloatRect AssetLoader::getMenuButtonBounds(int i)
{
    if (i < 0 || i >= 2)
    {
        return sf::FloatRect();
    }
    return menuButtonSprite[i]->getGlobalBounds();
}

sf::FloatRect AssetLoader::getAugmentButtonBounds(int i)
{
    if (i < 0 || i >= 3)
    {
        return sf::FloatRect();
    }
    return augmentButtonSprite[i]->getGlobalBounds();
}

void AssetLoader::drawMenu(sf::RenderWindow& window)
{
    if(hasMenuBackgroundSprite() && hasMenuButtonSprites()) 
    {
        window.draw(*menuBackgroundSprite);
        window.draw(*menuButtonSprite[0]);
        window.draw(*menuButtonSprite[1]);
    }
}

void AssetLoader::drawAugment(sf::RenderWindow& window)
{
    if(hasAugmentBackgroundSprite() && hasAugmentButtonSprites()) 
    {
        window.draw(*augmentBackgroundSprite);
        for(int i=0; i<3; i++)
            window.draw(*(augmentButtonSprite[i]));
    }
}

void AssetLoader::drawLevelBackground(sf::RenderWindow& window)
{
    if(hasLevelBackgroundSprite()) 
        window.draw(*levelBackgroundSprite);
}