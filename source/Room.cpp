#include "../headers/Room.h"
#include "../headers/Utils.h"

Room::Room(const bool &u, const bool &r, const bool &d, const bool &l, const sf::Texture &doorV, const sf::Texture &doorH, const sf::Texture &background)
    :backgroundSprite(background), up(nullptr), right(nullptr), down(nullptr), left(nullptr)
{
    if(u)
    {
        sf::Sprite door(doorV);
        doorUp.emplace(door);
    }

    if(r)
    {
        sf::Sprite door(doorH);
        doorRight.emplace(door);
    }

    if(d)
    {
        sf::Sprite door(doorV);
        doorDown.emplace(door);
    }

    if(l)
    {
        sf::Sprite door(doorH);
        doorLeft.emplace(door);
    }

    animationClock.reset();
}

void Room::load(Room *u, Room *r, Room *d, Room *l)
{
    up = u;
    right = r;
    down = d;
    left = l;
    if (doorUp)
    {
        doorUp->setOrigin({150, 0});
        doorUp->setPosition(sf::Vector2f(LOGICAL_WIDTH / 2, -66.5));
    }

    if (doorRight)
    {
        doorRight->setOrigin({300, 150});
        doorRight->setPosition(sf::Vector2f(LOGICAL_WIDTH + 66.5, LOGICAL_HEIGHT / 2));
    }

    if (doorDown)
    {
        doorDown->setOrigin({150, 0});
        doorDown->setPosition(sf::Vector2f(LOGICAL_WIDTH / 2, LOGICAL_HEIGHT + 66.5));
        doorDown->setScale({1, -1});
    }

    if (doorLeft)
    {
        doorLeft->setOrigin({300, 150});
        doorLeft->setPosition(sf::Vector2f(-66.5, LOGICAL_HEIGHT / 2));
        doorLeft->setScale({-1, 1});
    }

    for(int i = 0; i < 8; i++)
    {
        sf::RectangleShape wall;
        wall.setFillColor(sf::Color(255, 0, 0, 100));
        if (i < 4)
        {
            wall.setSize({900, 150});
            wall.setPosition(sf::Vector2f(i % 2 * 1020, i / 2 * 930));
        }
        else
        {
            wall.setSize({150, 330});
            wall.setPosition(sf::Vector2f(((i + 1) / 2) % 2 * 1770, i / 6 * 450  + 150));
        }
        walls.push_back(wall);
    }

    for(int i = 0; i < 4; i++)
    {
        if (i == 0 && up == nullptr)
        {
            sf::RectangleShape wall;
            wall.setFillColor(sf::Color(255, 0, 0, 100));
            wall.setSize({120, 150});
            wall.setPosition(sf::Vector2f(900, 0));
            walls.push_back(wall);
            continue;
        }
        else if (i == 1 && right == nullptr)
        {
            sf::RectangleShape wall;
            wall.setFillColor(sf::Color(255, 0, 0, 100));
            wall.setSize({150, 120});
            wall.setPosition(sf::Vector2f(1770, 480));
            walls.push_back(wall);
            continue;
        }
        else if (i == 2 && down == nullptr)
        {
            sf::RectangleShape wall;
            wall.setFillColor(sf::Color(255, 0, 0, 100));
            wall.setSize({120, 150});
            wall.setPosition(sf::Vector2f(900, 930));
            walls.push_back(wall);
            continue;
        }
        else if (i == 3 && left == nullptr)
        {
            sf::RectangleShape wall;
            wall.setFillColor(sf::Color(255, 0, 0, 100));
            wall.setSize({150, 120});
            wall.setPosition(sf::Vector2f(0, 480));
            walls.push_back(wall);
            continue;
        }
        sf::RectangleShape door;
        door.setFillColor(sf::Color(0, 255, 0, 100));
        if (i == 0 || i == 2)
        {
            door.setSize({120, 150});
            door.setPosition(sf::Vector2f(900, (i == 0) ? 0 : 930));
        }
        else
        {
            door.setSize({150, 120});
            door.setPosition(sf::Vector2f((i == 1) ? 1770 : 0, 480));
        }
        doors.push_back(door);
    }
}

void Room::draw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = getScaleFactor(window);
    sf::Sprite drawBg = backgroundSprite;
    drawBg.setPosition(mapToScreen(backgroundSprite.getPosition(), window));
    drawBg.scale(scaleFactor);
    window.draw(drawBg);

    if(doorUp)
    {
        sf::Sprite drawDoor = doorUp.value();
        drawDoor.setPosition(mapToScreen(doorUp.value().getPosition(), window));
        drawDoor.scale(scaleFactor);
        window.draw(drawDoor);
    }

    if(doorRight)
    {
        sf::Sprite drawDoor = doorRight.value();
        drawDoor.setPosition(mapToScreen(doorRight.value().getPosition(), window));
        drawDoor.scale(scaleFactor);
        window.draw(drawDoor);
    }

    if(doorDown)
    {
        sf::Sprite drawDoor = doorDown.value();
        drawDoor.setPosition(mapToScreen(doorDown.value().getPosition(), window));
        drawDoor.scale(scaleFactor);
        window.draw(drawDoor);
    }

    if(doorLeft)
    {
        sf::Sprite drawDoor = doorLeft.value();
        drawDoor.setPosition(mapToScreen(doorLeft.value().getPosition(), window));
        drawDoor.scale(scaleFactor);
        window.draw(drawDoor);
    }

    // for (auto &wall : walls)
    // {
    //     sf::RectangleShape drawWall = wall;
    //     drawWall.setPosition(mapToScreen(wall.getPosition(), window));
    //     drawWall.scale(scaleFactor);
    //     window.draw(drawWall);
    // }

    // for (auto &door : doors)
    // {
    //     sf::RectangleShape drawDoor = door;
    //     drawDoor.setPosition(mapToScreen(door.getPosition(), window));
    //     drawDoor.scale(scaleFactor);
    //     window.draw(drawDoor);
    // }
}

std::pair<int, Room*> Room::update(const Player &player, const bool &finished)
{
    int action = checkCollisions(player);
    if (finished)
        animationClock.start();
    
    if (animationClock.getElapsedTime().asSeconds() >= 1)
        open = true;

    switch (action)
    {
    case -2:
        return {-2, nullptr};
        break;

    case 0:
        return {0, up};
        break;

    case 1:
        return {1, right};
        break;

    case 2:
        return {2, down};
        break;

    case 3:
        return {3, left};
        break;

    default:
        return {-1, nullptr};
        break;
    }
}

int Room::checkCollisions(const Player &player)
{
    for (const auto &wall : walls)
    {
        if(player.boxCollidesWith(wall))
            return -2;
    }

    int doorDirections[4];
    int doorCount = 0;
    
    if (doorUp) doorDirections[doorCount++] = 0;
    if (doorRight) doorDirections[doorCount++] = 1;
    if (doorDown) doorDirections[doorCount++] = 2;
    if (doorLeft) doorDirections[doorCount++] = 3;
    
    for (int i = 0; i < doorCount; i++)
    {
        if (player.boxCollidesWith(doors[i]))
        {
            if (open)
                return doorDirections[i];
            else
                return -2;
        }
    }
    
    return -1;
}