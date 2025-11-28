#include "../headers/Minimap.h"
#include "../headers/Utils.h"

Minimap::Minimap()
    : currentPos({2, 3})
{
}

Minimap &Minimap::getInstance()
{
    static Minimap instance;
    return instance;
}

void Minimap::load(const int map[5][7])
{
    background.setSize({324, 150});
    background.setPosition({0,0});
    background.setFillColor(sf::Color(0, 0, 0, 100));

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            sf::RectangleShape cell;
            cell.setSize({42, 24});
            cell.setPosition(sf::Vector2f(
                2 + j * 47,
                2 + i * 29
            ));

            if (map[i][j] == 2)
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
    switch(moved)
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