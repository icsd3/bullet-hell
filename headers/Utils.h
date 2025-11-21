#pragma once
#include <SFML/Graphics.hpp>

constexpr float LOGICAL_WIDTH = 1920.0f;
constexpr float LOGICAL_HEIGHT = 1080.0f;

inline sf::Vector2f mapToScreen(const sf::Vector2f& logicalPos, const sf::RenderWindow& window) {
    return sf::Vector2f(
        logicalPos.x * window.getSize().x / LOGICAL_WIDTH,
        logicalPos.y * window.getSize().y / LOGICAL_HEIGHT
    );
}

inline sf::Vector2f mapToLogical(const sf::Vector2f& screenPos, const sf::RenderWindow& window) {
    return sf::Vector2f(
        screenPos.x * LOGICAL_WIDTH / window.getSize().x,
        screenPos.y * LOGICAL_HEIGHT / window.getSize().y
    );
}

inline sf::Vector2f getScaleFactor(const sf::RenderWindow& window) {
    return sf::Vector2f(
        1.f * window.getSize().x / LOGICAL_WIDTH,
        1.f * window.getSize().y / LOGICAL_HEIGHT
    );
}
