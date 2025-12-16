#pragma once

#include "../headers/Player.h"
#include "../headers/TextureManager.h"

class Augment
{
    std::string fontPath;

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

    void load();
    void draw(sf::RenderWindow &);
    int handleInput(const sf::RenderWindow &, const sf::Event &);
    // void applyModifiers(Player &);
};