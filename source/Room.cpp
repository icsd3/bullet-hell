#include "../headers/Room.h"

Room::Room(const sf::Texture &dv, const sf::Texture &dh, const sf::Texture &background, Player &player)
    : player(player), backgroundSprite(background), doorVertical(&dv), doorHorizontal(&dh), grid{{0}}
{
    animationClock.reset();
}

void Room::doLoad(std::weak_ptr<Room> u, std::weak_ptr<Room> r, std::weak_ptr<Room> d, std::weak_ptr<Room> l)
{
    up = u;
    right = r;
    down = d;
    left = l;

    for (int i = 0; i < 8; i++)
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
            position = sf::Vector2f(((i + 1) / 2) % 2 * 1770, i / 6 * 450 + 150);
        }

        Collider wall(position);
        walls.push_back(wall);
        walls.back().load(size);
    }

    for (int i = 0; i < 4; i++)
    {
        sf::Vector2f position;
        sf::Vector2f size;

        if (i == 0 && up.expired())
        {
            size = {120, 150};
            position = {900, 0};
            Collider wall(position);
            walls.push_back(wall);
            walls.back().load(size);
            continue;
        }

        else if (i == 1 && right.expired())
        {
            size = {150, 120};
            position = {1770, 480};
            Collider wall(position);
            walls.push_back(wall);
            walls.back().load(size);
            continue;
        }

        else if (i == 2 && down.expired())
        {
            size = {120, 150};
            position = {900, 930};
            Collider wall(position);
            walls.push_back(wall);
            walls.back().load(size);
            continue;
        }

        else if (i == 3 && left.expired())
        {
            size = {150, 120};
            position = {0, 480};
            Collider wall(position);
            walls.push_back(wall);
            walls.back().load(size);
            continue;
        }

        const sf::Texture *texturePointer;

        if (i == 0 || i == 2)
        {
            size = {120, 150};
            position = sf::Vector2f(960, (i == 0) ? 75 : 1005);
            texturePointer = doorVertical;
        }
        else
        {
            size = {120, 150};
            position = sf::Vector2f((i == 1) ? 1845 : 75, 540);
            texturePointer = doorHorizontal;
        }

        const sf::Texture &texture = *texturePointer;

        Door door(position, texture, i);
        doors.push_back(door);
        doors.back().doorLoad(size);
    }
}

void Room::load(std::weak_ptr<Room> u, std::weak_ptr<Room> r, std::weak_ptr<Room> d, std::weak_ptr<Room> l)
{
    doLoad(u, r, d, l);
}

void Room::doDraw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);
    sf::Sprite drawBg = backgroundSprite;
    drawBg.setPosition(Utils::mapToScreen(backgroundSprite.getPosition(), window));
    drawBg.scale(scaleFactor);
    window.draw(drawBg);

    for (auto &door : doors)
        door.draw(window);

    for (auto &wall : walls)
        wall.draw(window);

    player.draw(window);

    for (auto &projectile : playerProjectiles)
        projectile.draw(window);
}

void Room::draw(sf::RenderWindow &window)
{
    doDraw(window);
}

void Room::animate(const unsigned int &frame)
{
    for (auto &door : doors)
    {
        door.update(frame);
    }
}

std::pair<int, std::weak_ptr<Room>> Room::doUpdate(const float &dt)
{
    int action = doCheckPlayerCollisions();

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

    for (size_t i = 0; i < playerProjectiles.size();)
    {
        if (playerProjectiles[i].update(dt))
            playerProjectiles.erase(playerProjectiles.begin() + i);

        else
        {
            if (doCheckEntityCollisions(playerProjectiles[i]))
                playerProjectiles.erase(playerProjectiles.begin() + i);

            else
                i++;
        }
    }

    std::weak_ptr<Room> nothing;

    switch (action)
    {
    case -2:
        return {-2, nothing};
        break;

    case 0:
        playerProjectiles.clear();
        return {0, up};
        break;

    case 1:
        playerProjectiles.clear();
        return {1, right};
        break;

    case 2:
        playerProjectiles.clear();
        return {2, down};
        break;

    case 3:
        playerProjectiles.clear();
        return {3, left};
        break;

    default:
        return {-1, nothing};
        break;
    }
}

std::pair<int, std::weak_ptr<Room>> Room::update(const float &dt)
{
    return doUpdate(dt);
}

void Room::spawnPlayerProjectile(const sf::Vector2f &target)
{
    std::vector<Projectile> bullets = player.fire(target);

    for (const auto &bullet : bullets)
    {
        playerProjectiles.push_back(bullet);
        playerProjectiles.back().load();
    }
}

int Room::doCheckPlayerCollisions()
{
    for (const auto &wall : walls)
        if (player.collidesWith(wall))
            return -2;

    int doorDirections[4];
    int doorCount = 0;

    if (!up.expired())
        doorDirections[doorCount++] = 0;

    if (!right.expired())
        doorDirections[doorCount++] = 1;

    if (!down.expired())
        doorDirections[doorCount++] = 2;

    if (!left.expired())
        doorDirections[doorCount++] = 3;

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

int Room::checkPlayerCollisions()
{
    return doCheckPlayerCollisions();
}

bool Room::doCheckEntityCollisions(const Entity &entity)
{
    for (const auto &wall : walls)
        if (entity.collidesWith(wall))
            return true;

    for (const auto &door : doors)
        if (entity.collidesWith(door))
            return true;

    return false;
}

bool Room::checkEntityCollisions(const Entity &entity)
{
    return doCheckEntityCollisions(entity);
}

void Room::doStart()
{
    if (!open)
        animationClock.start();
}

void Room::start()
{
    doStart();
}

std::ostream &operator<<(std::ostream &os, const Room &room)
{
    os << "Room (Open: " << (room.open ? "Yes" : "No") << ", Neighbors: ";
    if (!room.up.expired())
        os << "Up ";
    if (!room.right.expired())
        os << "Right ";
    if (!room.down.expired())
        os << "Down ";
    if (!room.left.expired())
        os << "Left ";
    os << ")\n";
    os << "    Player Projectiles:\n";
    os << "        Count: " << room.playerProjectiles.size() << "\n";
    if (!room.playerProjectiles.empty())
        for (size_t i = 0; i < room.playerProjectiles.size(); i++)
            os << "        Projectile " << i + 1 << ":\n            " << room.playerProjectiles[i] << "\n\n";
    return os;
}