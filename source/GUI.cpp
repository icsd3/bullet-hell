#include "../headers/GUI.h"

GUI::GUI()
    : health(font), fps(font)
{
    if (!font.openFromFile(fontPath))
    {
        std::cerr << "Error loading arial.ttf\n";
    }
}

GUI &GUI::getInstance()
{
    static GUI instance;
    return instance;
}

std::ostream &operator<<(std::ostream &os, const GUI &gui)
{
    os << "    GUI Font loaded: " << (gui.font.getInfo().family.empty() ? "No" : gui.fontPath) << "\n";
    os << "    Health Text: " << gui.health.getString().toAnsiString();
    os << "    FPS Text: " << gui.fps.getString().toAnsiString();
    return os;
}

void GUI::loadGUI(sf::Window &window)
{
    maxHealthBar.setSize(sf::Vector2f(0.4f * window.getSize().x, 0.04f * window.getSize().y));
    maxHealthBar.setOrigin(sf::Vector2f(0.5f * maxHealthBar.getLocalBounds().size.x, 0.5f * maxHealthBar.getLocalBounds().size.y));
    maxHealthBar.setPosition(sf::Vector2f(0.5f * window.getSize().x, window.getSize().y - 0.5f * maxHealthBar.getSize().y - 3.f));
    maxHealthBar.setFillColor(sf::Color(0, 50, 0, 175));
    maxHealthBar.setOutlineThickness(3.f);
    maxHealthBar.setOutlineColor(sf::Color::Black);

    currentHealthBar.setSize(maxHealthBar.getSize());
    currentHealthBar.setOrigin(sf::Vector2f(0, 0.5f * currentHealthBar.getLocalBounds().size.y));
    currentHealthBar.setPosition(sf::Vector2f(maxHealthBar.getPosition().x - 0.5f * maxHealthBar.getSize().x, maxHealthBar.getPosition().y));
    currentHealthBar.setFillColor(sf::Color(0, 100, 0, 175));

    health.setCharacterSize(30);
    health.setFillColor(sf::Color::Green);
    health.setOutlineThickness(2.f);
    health.setOutlineColor(sf::Color::Black);

    fps.setCharacterSize(20);
    fps.setFillColor(sf::Color::Green);
    fps.setPosition(sf::Vector2f(window.getSize().x - 100.f, 10.f));
}

void GUI::updateGUI(const sf::Vector2i &HP, sf::RenderWindow &window)
{
    currentHealthBar.setSize(sf::Vector2f(static_cast<float>(HP.x) / static_cast<float>(HP.y) * maxHealthBar.getSize().x, currentHealthBar.getSize().y));

    health.setString("HP: " + std::to_string(HP.x) + " / " + std::to_string(HP.y));
    health.setOrigin(sf::Vector2f(health.getLocalBounds().position.x + 0.5f * health.getLocalBounds().size.x, health.getLocalBounds().position.y + 0.5f * health.getLocalBounds().size.y));
    health.setPosition(maxHealthBar.getPosition());

    if (fpsClock.getElapsedTime().asSeconds() >= 1.f)
    {
        fps.setString("FPS: " + std::to_string(frameCount));
        fps.setOrigin(sf::Vector2f(fps.getLocalBounds().position.x + fps.getLocalBounds().size.x, fps.getLocalBounds().position.y));
        fps.setPosition(sf::Vector2f(window.getSize().x - 10.f, 10.f));
        frameCount = 0;
        fpsClock.restart();
    }
    else
    {
        frameCount++;
    }
}

void GUI::draw(sf::RenderWindow &window)
{
    window.draw(maxHealthBar);
    window.draw(currentHealthBar);
    window.draw(health);
    window.draw(fps);
}