#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

class mainMenu
{
    mainMenu();
    mainMenu(const mainMenu &) = delete;
    mainMenu &operator=(const mainMenu &) = delete;

    std::string backgroundPath = "textures/menu_background.png";
    std::string startButtonPath = "textures/start_button.png";
    std::string settingsButtonPath = "textures/settings_button.png";
    std::string exitButtonPath = "textures/exit_button.png";

    sf::Texture backgroundTexture;
    sf::Texture startButtonTexture;
    sf::Texture settingsButtonTexture;
    sf::Texture exitButtonTexture;

    sf::Sprite backgroundSprite;
    sf::Sprite startButtonSprite;
    sf::Sprite settingsButtonSprite;
    sf::Sprite exitButtonSprite;

public:
    static mainMenu &getInstance();
    ~mainMenu() = default;
    friend std::ostream &operator<<(std::ostream &os, const mainMenu &menu);

    void load(sf::RenderWindow &);
    void draw(sf::RenderWindow &);
    int handleInput(const sf::Event &event);
};