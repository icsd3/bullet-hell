#include "../headers/Settings.h"

Settings::Settings()
    : screenText(font), controlsText(font)
{
    if (!font.openFromFile("fonts/courier.ttf"))
    {
        std::cerr << "Error loading courier.ttf\n";
    }
}

Settings &Settings::getInstance()
{
    static Settings instance;
    return instance;
}

void Settings::loadSettingsBox(sf::RenderWindow &window, const bool &isFullscreen, const bool &areControlsMouse)
{
    fullscreen = isFullscreen;
    controls = areControlsMouse;

    settingsBox.setSize(sf::Vector2f(0.5f * window.getSize().x, 0.5f * window.getSize().y));
    settingsBox.setOrigin(sf::Vector2f(0.5f * settingsBox.getLocalBounds().size.x, 0.5f * settingsBox.getLocalBounds().size.y));
    settingsBox.setPosition(sf::Vector2f(0.5f * window.getSize().x, 0.5f * window.getSize().y));
    settingsBox.setFillColor(sf::Color(0, 0, 0, 200));
    settingsBox.setOutlineThickness(3.f);
    settingsBox.setOutlineColor(sf::Color::Black);

    screenText.setCharacterSize(30);
    screenText.setFillColor(sf::Color::White);
    screenText.setString("Window Mode: " + std::string(fullscreen ? "Fullscreen" : "Windowed"));
    screenText.setOrigin(sf::Vector2f(screenText.getLocalBounds().position.x + 0.5f * screenText.getLocalBounds().size.x,
                                      screenText.getLocalBounds().position.y + 0.5f * screenText.getLocalBounds().size.y));
    screenText.setPosition(sf::Vector2f(settingsBox.getPosition().x, settingsBox.getPosition().y - 50.f));

    controlsText.setCharacterSize(30);
    controlsText.setFillColor(sf::Color::White);
    controlsText.setString("Controls: " + std::string(controls ? "Mouse" : "WASD"));
    controlsText.setOrigin(sf::Vector2f(controlsText.getLocalBounds().position.x + 0.5f * controlsText.getLocalBounds().size.x,
                                        controlsText.getLocalBounds().position.y + 0.5f * controlsText.getLocalBounds().size.y));
    controlsText.setPosition(sf::Vector2f(settingsBox.getPosition().x, settingsBox.getPosition().y + 50.f));
}

void Settings::draw(sf::RenderWindow &window, const bool &isFullscreen, const bool &areControlsMouse)
{
    fullscreen = isFullscreen;
    controls = areControlsMouse;
    window.draw(settingsBox);
    window.draw(screenText);
    window.draw(controlsText);
}

std::ostream &operator<<(std::ostream &os, const Settings &settings)
{
    os << "    Fullscreen: " << (settings.fullscreen ? "Enabled" : "Disabled") << "\n";
    os << "    Controls: " << (settings.controls ? "Enabled" : "Disabled") << "\n";
    return os;
}