#include "../headers/GUI.h"

GUI::GUI()
    :health(font)
{
    if(!font.openFromFile(fontPath))
    {
        std::cerr << "Error loading arial.ttf\n";
    }
}

GUI& GUI::getInstance()
{
    static GUI instance;
    return instance;
}

std::ostream& operator<<(std::ostream& os, const GUI& gui)
{
    os << "    GUI Font loaded: " << (gui.font.getInfo().family.empty() ? "No" : gui.fontPath) << "\n";
    os << "    Health Text: " << gui.health.getString().toAnsiString();
    return os;
}

void GUI::loadGUI()
{
    health.setCharacterSize(40);
    health.setFillColor(sf::Color::Red);
    health.setPosition(sf::Vector2f(10.f, 10.f));
}

void GUI::updateGUI(sf::Vector2i HP)
{
    health.setString("HP: " + std::to_string(HP.x) + " / " + std::to_string(HP.y));
}

void GUI::draw(sf::RenderWindow& window)
{
    window.draw(health);
}