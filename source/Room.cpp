#include "../headers/Room.h"
#include "../headers/Utils.h"

Room::Room(const sf::Texture &dv, const sf::Texture &dh, const sf::Texture &background)
    :backgroundSprite(background), doorVertical(dv), doorHorizontal(dh), up(nullptr), right(nullptr), down(nullptr), left(nullptr)
{
    animationClock.reset();
}

void Room::load(Room *u, Room *r, Room *d, Room *l)
{
    up = u;
    right = r;
    down = d;
    left = l;

    for(int i = 0; i < 8; i++)
    {
        sf::Vector2f position;
        sf::Vector2f size;
        
        if (i < 4)
        {
            size = {900, 150};
            position = sf::Vector2f(i % 2 * 1020, i / 2 * 930);
        }
        else
        {
            size = {150, 330};
            position = sf::Vector2f(((i + 1) / 2) % 2 * 1770, i / 6 * 450  + 150);
        }

        Object wall(position, false, size);
        walls.push_back(wall);
        walls.back().load();
    }

    for(int i = 0; i < 4; i++)
    {
        sf::Vector2f position;
        sf::Vector2f size;

        if (i == 0 && up == nullptr)
        {
            size = {120, 150};
            position = {900, 0};
            Object wall(position, false, size);
            walls.push_back(wall);
            walls.back().load();
            continue;
        }

        else if (i == 1 && right == nullptr)
        {
            size = {150, 120};
            position = {1770, 480};
            Object wall(position, false, size);
            walls.push_back(wall);
            walls.back().load();
            continue;
        }

        else if (i == 2 && down == nullptr)
        {
            size = {120, 150};
            position = {900, 930};
            Object wall(position, false, size);
            walls.push_back(wall);
            walls.back().load();
            continue;
        }
        
        else if (i == 3 && left == nullptr)
        {
            size = {150, 120};
            position = {0, 480};
            Object wall(position, false, size);
            walls.push_back(wall);
            walls.back().load();
            continue;
        }

        bool orientation = false;
        const sf::Texture *texturePointer;

        if (i == 0 || i == 2)
        {
            size = {120, 150};
            position = sf::Vector2f(960, (i == 0) ? 75 : 1005);
            texturePointer = &doorVertical;
        }
        else
        {
            size = {120, 150};
            position = sf::Vector2f((i == 1) ? 1845 : 75, 540);
            texturePointer = &doorHorizontal;
        }

        const sf::Texture &texture = *texturePointer;

        Door door(position, orientation, texture, size, i);
        doors.push_back(door);
    }
}

void Room::draw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);
    sf::Sprite drawBg = backgroundSprite;
    drawBg.setPosition(Utils::mapToScreen(backgroundSprite.getPosition(), window));
    drawBg.scale(scaleFactor);
    window.draw(drawBg);

    for (auto &door : doors)
    {
        door.draw(window);
    }

    for (auto &wall : walls)
    {
        wall.draw(window);
    }
}

void Room::animate(const int &frame)
{
    for (auto &door : doors)
    {
        door.update(frame);
    }
}

std::pair<int, Room*> Room::update(const Player &player, const bool &finished)
{
    int action = checkCollisions(player);

    if (finished)
        animationClock.start();
    
    if (animationClock.getElapsedTime().asMilliseconds() >= 600)
    {
        open = true;
        animationClock.stop();
        animate(4);
    }

    else if (animationClock.getElapsedTime().asMilliseconds() >= 450)
        animate(3);

    else if (animationClock.getElapsedTime().asMilliseconds() >= 300)
        animate(2);

    else if (animationClock.getElapsedTime().asMilliseconds() >= 150)
        animate(1);

    else
        animate(0);

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
        if(player.collidesWith(wall))
            return -2;
    }

    int doorDirections[4];
    int doorCount = 0;
    
    if (up != nullptr) doorDirections[doorCount++] = 0;
    if (right != nullptr) doorDirections[doorCount++] = 1;
    if (down != nullptr) doorDirections[doorCount++] = 2;
    if (left != nullptr) doorDirections[doorCount++] = 3;
    
    for (int i = 0; i < doorCount; i++)
    {
        if (player.collidesWith(doors[i]))
        {
            if (open)
                return doorDirections[i];
            else
                return -2;
        }
    }
    
    return -1;
}

std::ostream &operator<<(std::ostream &os, const Room &room)
{
    os << "Room (Open: " << (room.open ? "Yes" : "No") << ", Neighbors: ";
    if (room.up) os << "Up ";
    if (room.right) os << "Right ";
    if (room.down) os << "Down ";
    if (room.left) os << "Left ";
    os << ")";
    return os;
}