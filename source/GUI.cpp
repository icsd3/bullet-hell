#include "../headers/GUI.h"

GUI::GUI()
    :health(font)
{
    if(!font.openFromFile("fonts/arial.ttf"))
    {
        std::cerr << "Error loading arial.ttf\n";
    }
    health.setFont(font);
}

GUI& GUI::getInstance()
{
    static GUI instance;
    return instance;
}

void GUI::loadGUI()
{
    if(!font.openFromFile("fonts/arial.ttf"))
    {
        std::cerr << "Error loading arial.ttf\n";
    }
    health.setFont(font);
    health.setCharacterSize(40);
    health.setFillColor(sf::Color::Red);
    health.setPosition(sf::Vector2f(10.f, 10.f));
}

void GUI::updateGUI(int currentHealth, int maxHealth)
{
    health.setString("HP: " + std::to_string(currentHealth) + " / " + std::to_string(maxHealth));
}