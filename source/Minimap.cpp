#include "../headers/Minimap.h"

Minimap::Minimap()
    : currentPos({4, 3})
{
}

Minimap &Minimap::getInstance()
{
    static Minimap instance;
    return instance;
}

void Minimap::load(const int map[5][7])
{
    background.setSize({334, 150});
    background.setPosition({0, 0});
    background.setFillColor(sf::Color(0, 0, 0, 200));

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            sf::RectangleShape cell;
            cell.setSize({42, 24});
            cell.setPosition(sf::Vector2f(
                5 + j * 47,
                5 + i * 29));

            if (map[i][j] == 3)
                cell.setFillColor(sf::Color(150, 0, 0, 255));

            else if (map[i][j] == 2)
                cell.setFillColor(sf::Color(0, 150, 0, 255));

            else if (map[i][j] == 1)
                cell.setFillColor(sf::Color(100, 100, 100, 255));

            else
                cell.setFillColor(sf::Color(50, 50, 50, 255));

            grid.emplace_back(cell);
        }
    }
}

void Minimap::update(const int &moved)
{
    switch (moved)
    {
    case 0:
        grid[currentPos.first * 7 + currentPos.second].setFillColor(sf::Color(100, 100, 100, 255));
        currentPos.first -= 1;
        grid[currentPos.first * 7 + currentPos.second].setFillColor(sf::Color(0, 150, 0, 255));
        break;

    case 1:
        grid[currentPos.first * 7 + currentPos.second].setFillColor(sf::Color(100, 100, 100, 255));
        currentPos.second += 1;
        grid[currentPos.first * 7 + currentPos.second].setFillColor(sf::Color(0, 150, 0, 255));
        break;

    case 2:
        grid[currentPos.first * 7 + currentPos.second].setFillColor(sf::Color(100, 100, 100, 255));
        currentPos.first += 1;
        grid[currentPos.first * 7 + currentPos.second].setFillColor(sf::Color(0, 150, 0, 255));
        break;

    case 3:
        grid[currentPos.first * 7 + currentPos.second].setFillColor(sf::Color(100, 100, 100, 255));
        currentPos.second -= 1;
        grid[currentPos.first * 7 + currentPos.second].setFillColor(sf::Color(0, 150, 0, 255));
        break;

    default:
        break;
    }
}

void Minimap::draw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    sf::RectangleShape drawBg = background;
    drawBg.setPosition(Utils::mapToScreen(background.getPosition(), window));
    drawBg.setScale(scaleFactor);
    window.draw(drawBg);

    for (const auto &cell : grid)
    {
        sf::RectangleShape drawCell = cell;
        drawCell.setPosition(Utils::mapToScreen(cell.getPosition(), window));
        drawCell.setScale(scaleFactor);
        window.draw(drawCell);
    }
}

std::ostream &operator<<(std::ostream &os, const Minimap &minimap)
{
    os << "Minimap (Current Position: (" << minimap.currentPos.first << ", " << minimap.currentPos.second << "))";
    return os;
}