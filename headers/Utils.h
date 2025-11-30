#pragma once
#include <SFML/Graphics.hpp>
#include <random>

constexpr float LOGICAL_WIDTH = 1920.0f;
constexpr float LOGICAL_HEIGHT = 1080.0f;

class Utils
{
    static bool paused;
    static int displayBoxes;
    static std::mt19937 rng;

public:
    static sf::Vector2f mapToScreen(const sf::Vector2f&, const sf::RenderWindow&);
    static sf::Vector2f mapToLogical(const sf::Vector2f&, const sf::RenderWindow&);
    static sf::Vector2f getScaleFactor(const sf::RenderWindow&);
    static std::mt19937 &getRng();
    static bool changePaused(const bool);
    static int changeDisplayBoxes(const bool);
};
