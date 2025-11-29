#include "../headers/Utils.h"

#include <SFML/Graphics.hpp>

std::mt19937 Utils::rng(std::random_device{}());
bool Utils::paused = false;

sf::Vector2f Utils::mapToScreen(const sf::Vector2f& logicalPos, const sf::RenderWindow& window) {
    return sf::Vector2f(
        logicalPos.x * static_cast<float>(window.getSize().x) / LOGICAL_WIDTH,
        logicalPos.y * static_cast<float>(window.getSize().y) / LOGICAL_HEIGHT
    );
}

sf::Vector2f Utils::mapToLogical(const sf::Vector2f& screenPos, const sf::RenderWindow& window) {
    return sf::Vector2f(
        screenPos.x * LOGICAL_WIDTH / static_cast<float>(window.getSize().x),
        screenPos.y * LOGICAL_HEIGHT / static_cast<float>(window.getSize().y)
    );
}

sf::Vector2f Utils::getScaleFactor(const sf::RenderWindow& window) {
    return sf::Vector2f(
        1.f * static_cast<float>(window.getSize().x) / LOGICAL_WIDTH,
        1.f * static_cast<float>(window.getSize().y) / LOGICAL_HEIGHT
    );
}

std::mt19937 &Utils::getRng()
{
    return rng;
}

bool Utils::changePaused(const bool change)
{
    if (change)
        paused = !paused;
    return paused;
}