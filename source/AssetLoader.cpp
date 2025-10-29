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

void AssetLoader::loadStaticAssets()
{
    if (!menuBackgroundTexture.loadFromFile("textures/menu_background.png"))
    {
        std::cerr << "Error loading menu_background.png\n";
    }
    menuBackgroundSprite.emplace(menuBackgroundTexture);

    for(int i = 0; i<2; i++)
    {
         if(i==0)
        {
            if (!menuButtonTexture[i].loadFromFile("textures/start_button.png"))
            {
                std::cerr << "Error loading start_button.png\n";
            }
        }
        else
        {
            if (!menuButtonTexture[i].loadFromFile("textures/exit_button.png"))
            {
                std::cerr << "Error loading exit_button.png\n";
            }
        }
        menuButtonSprite[i].emplace(menuButtonTexture[i]);
    }

    if (!augmentBackgroundTexture.loadFromFile("textures/augment_background.png"))
    {
        std::cerr << "Error loading augment_background.png\n";
    }
    augmentBackgroundSprite.emplace(augmentBackgroundTexture);

    for(int i = 0; i<3; i++)
    {
        if (!augmentButtonTexture[i].loadFromFile("textures/augment_button.png"))
        {
            std::cerr << "Error loading augment_button.png\n";
        }
        augmentButtonSprite[i].emplace(augmentButtonTexture[i]);
    }

    if(!levelBackgroundTexture.loadFromFile("textures/level_background.png"))
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