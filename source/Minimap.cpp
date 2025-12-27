#include "../headers/Minimap.h"

Minimap::Minimap()
    : currentPos({4, 3})
{
}

void Minimap::load(const int map[5][7])
{
    background.setSize({334, 150});
    background.setPosition({0, 0});
    background.setFillColor(sf::Color(0, 0, 0, 200));

    grid.clear();
    revealed.clear();
    currentPos = {4, 3};

    int maxId = 0;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 7; j++)
            if (map[i][j] > maxId) maxId = map[i][j];

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            mapData[i][j] = map[i][j];
            sf::RectangleShape cell;
            cell.setSize({42, 24});
            cell.setPosition(sf::Vector2f(
                5 + j * 47,
                5 + i * 29));

            if (i == currentPos.first && j == currentPos.second)
                cell.setFillColor(sf::Color(0, 150, 0, 255));
            else if (map[i][j] == maxId && maxId > 1)
            {
                cell.setFillColor(sf::Color(50, 50, 50, 255));
                cell.setOutlineColor(sf::Color(150, 0, 0, 255));
                cell.setOutlineThickness(-2);
            }
            else if (map[i][j] > 0)
                cell.setFillColor(sf::Color(50, 50, 50, 255));

            grid.emplace_back(cell);
            revealed.push_back(false);
        }
    }
    revealed[currentPos.first * 7 + currentPos.second] = true;
    revealNeighbors(currentPos.first, currentPos.second);
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
    revealed[currentPos.first * 7 + currentPos.second] = true;
    revealNeighbors(currentPos.first, currentPos.second);
}

void Minimap::draw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    sf::RectangleShape drawBg = background;
    drawBg.setPosition(Utils::mapToScreen(background.getPosition(), window));
    drawBg.setScale(scaleFactor);
    window.draw(drawBg);

    for (size_t i = 0; i < grid.size(); ++i)
    {
        if (revealed[i])
        {
            sf::RectangleShape drawCell = grid[i];
            drawCell.setPosition(Utils::mapToScreen(grid[i].getPosition(), window));
            drawCell.setScale(scaleFactor);
            window.draw(drawCell);
        }
    }
}

std::ostream &operator<<(std::ostream &os, const Minimap &minimap)
{
    os << "Minimap (Current Position: (" << minimap.currentPos.first << ", " << minimap.currentPos.second << "))";
    return os;
}

void Minimap::revealNeighbors(int r, int c)
{
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++)
    {
        int nr = r + dr[i];
        int nc = c + dc[i];

        if (nr >= 0 && nr < 5 && nc >= 0 && nc < 7)
        {
            if (mapData[nr][nc] > 0)
            {
                revealed[nr * 7 + nc] = true;
            }
        }
    }
}