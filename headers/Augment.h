#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

class Augment
{
    Augment();
    Augment(const Augment &) = delete;
    Augment &operator=(const Augment &) = delete;

    std::string backgroundPath = "textures/augment_background.png";
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
    static Augment &getInstance();
    ~Augment() = default;
    friend std::ostream &operator<<(std::ostream &os, const Augment &Augment);

    void loadMainMenu(sf::RenderWindow &);
    void drawMainMenu(sf::RenderWindow &);
    int handleInput(const sf::Event &event, const sf::RenderWindow &window);
};