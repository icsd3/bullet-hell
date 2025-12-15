#pragma once

#include "../headers/Player.h"

class Augment
{
    std::string backgroundPath;
    std::string buttonPath;
    std::string fontPath;

    sf::Texture backgroundTexture;
    sf::Texture buttonTexture;
    sf::Font font;

    sf::Sprite backgroundSprite;
    sf::Sprite firstButtonSprite;
    sf::Text firstButtonText;
    sf::Sprite secondButtonSprite;
    sf::Text secondButtonText;
    sf::Sprite thirdButtonSprite;
    sf::Text thirdButtonText;

public:
    Augment();
    Augment(const Augment &) = delete;
    Augment &operator=(const Augment &) = delete;
    ~Augment() = default;
    friend std::ostream &operator<<(std::ostream &, const Augment &);

    void load();
    void draw(sf::RenderWindow &);
    int handleInput(const sf::RenderWindow &, const sf::Event &);
    // void applyModifiers(Player &);
};