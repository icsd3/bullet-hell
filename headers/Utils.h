#pragma once
#include <SFML/Graphics.hpp>
#include <random>

constexpr float LOGICAL_WIDTH = 1920.0f;
constexpr float LOGICAL_HEIGHT = 1080.0f;

static std::mt19937 rng(std::random_device{}());

inline sf::Vector2f mapToScreen(const sf::Vector2f& logicalPos, const sf::RenderWindow& window) {
    return sf::Vector2f(
        logicalPos.x * static_cast<float>(window.getSize().x) / LOGICAL_WIDTH,
        logicalPos.y * static_cast<float>(window.getSize().y) / LOGICAL_HEIGHT
    );
}

inline sf::Vector2f mapToLogical(const sf::Vector2f& screenPos, const sf::RenderWindow& window) {
    return sf::Vector2f(
        screenPos.x * LOGICAL_WIDTH / static_cast<float>(window.getSize().x),
        screenPos.y * LOGICAL_HEIGHT / static_cast<float>(window.getSize().y)
    );
}

inline sf::Vector2f getScaleFactor(const sf::RenderWindow& window) {
    return sf::Vector2f(
        1.f * static_cast<float>(window.getSize().x) / LOGICAL_WIDTH,
        1.f * static_cast<float>(window.getSize().y) / LOGICAL_HEIGHT
    );
}
