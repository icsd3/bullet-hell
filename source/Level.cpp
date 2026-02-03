#include "../headers/Level.h"

Level::Level(Player &p, GUI &g)
    : player(p), gui(g), target(sf::Vector2f(LOGICAL_WIDTH / 2.f, LOGICAL_HEIGHT * 0.8f)), map{{0}}
{
}

void Level::generateRooms(const int n)
{
    RoomFactory factory(player);

    map[4][3] = 1;

    std::vector<std::pair<int, int>> frontier;
    frontier.emplace_back(4, 3);

    std::vector<std::pair<int, int>> dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    int cellCount = 1;

    std::mt19937 &rng = Utils::getRng();

    while (cellCount < n)
    {
        if (frontier.empty())
        {
            std::fill(&map[0][0], &map[0][0] + 35, false);
            generateRooms(n);
            return;
        }
        std::uniform_int_distribution<int> distF(0, frontier.size() - 1);
        auto [x1, y1] = frontier[distF(rng)];

        std::vector<std::pair<int, int>> moves;

        for (const auto &d : dirs)
        {
            int nx = x1 + d.first;
            int ny = y1 + d.second;

            if (nx < 0 || ny < 0 || nx >= MAP_SIZE_Y || ny >= MAP_SIZE_X)
                continue;

            if (map[nx][ny])
                continue;

            int count = 0;

            for (const auto &i : dirs)
            {
                int mx = nx + i.first;
                int my = ny + i.second;

                if (mx < 0 || my < 0 || mx >= MAP_SIZE_Y || my >= MAP_SIZE_X)
                    continue;

                if (map[mx][my])
                    count++;
            }

            if (count > 1)
                continue;

            moves.emplace_back(nx, ny);
        }

        if (moves.empty())
        {
            frontier.erase(frontier.begin() + distF(rng));
            continue;
        }

        std::uniform_int_distribution<int> distM(0, moves.size() - 1);
        auto [nx, ny] = moves[distM(rng)];

        map[nx][ny] = 1;
        cellCount++;
        frontier.emplace_back(nx, ny);
    }

    rooms.clear();

    std::queue<std::pair<int, int>> q;
    bool visited[MAP_SIZE_Y][MAP_SIZE_X] = {{false}};

    q.push({4, 3});
    visited[4][3] = true;

    while (!q.empty())
    {
        auto [i, j] = q.front();
        q.pop();

        bool up = (i > 0) && map[i - 1][j];
        bool down = (i < 4) && map[i + 1][j];
        bool left = (j > 0) && map[i][j - 1];
        bool right = (j < 6) && map[i][j + 1];

        map[i][j] = rooms.size() + 1;

        std::shared_ptr<Room> room;
        
        if (rooms.size() == 0)
            room = factory.createRoom(RoomType::Start);
        else if (rooms.size() < static_cast<size_t>(n - 1))
            room = factory.createRoom(RoomType::Enemy);
        else
            room = factory.createRoom(RoomType::Boss);

        rooms.push_back(room);

        if (up && !visited[i - 1][j])
        {
            visited[i - 1][j] = true;
            q.push({i - 1, j});
        }
        if (down && !visited[i + 1][j])
        {
            visited[i + 1][j] = true;
            q.push({i + 1, j});
        }
        if (left && !visited[i][j - 1])
        {
            visited[i][j - 1] = true;
            q.push({i, j - 1});
        }
        if (right && !visited[i][j + 1])
        {
            visited[i][j + 1] = true;
            q.push({i, j + 1});
        }
    }
}

void Level::load(const int nr)
{
    std::uniform_int_distribution<int> dist(5 + nr * 2, 8 + nr * 2);
    std::mt19937 &rng = Utils::getRng();
    int n = dist(rng);
    generateRooms(n);
    
    if (rooms.empty())
        throw OutOfBoundsException("No rooms were generated for the level");
        
    currentRoom = rooms[0];

    for (int i = 0; i < MAP_SIZE_Y; i++)
    {
        for (int j = 0; j < MAP_SIZE_X; j++)
        {
            std::weak_ptr<Room> up;
            std::weak_ptr<Room> down;
            std::weak_ptr<Room> left;
            std::weak_ptr<Room> right;
            if (map[i][j] > 0)
            {
                size_t currentIndex = static_cast<size_t>(map[i][j] - 1);
                if (currentIndex >= rooms.size())
                    throw OutOfBoundsException("Invalid room index in Level::load during neighbor setup");

                if (i - 1 >= 0 && map[i - 1][j] > 0)
                {
                    size_t upIndex = static_cast<size_t>(map[i - 1][j] - 1);
                    up = rooms.at(upIndex);
                }
                if (i + 1 < MAP_SIZE_Y && map[i + 1][j] > 0)
                {
                    size_t downIndex = static_cast<size_t>(map[i + 1][j] - 1);
                    down = rooms.at(downIndex);
                }
                if (j - 1 >= 0 && map[i][j - 1] > 0)
                {
                    size_t leftIndex = static_cast<size_t>(map[i][j - 1] - 1);
                    left = rooms.at(leftIndex);
                }
                if (j + 1 < MAP_SIZE_X && map[i][j + 1] > 0)
                {
                    size_t rightIndex = static_cast<size_t>(map[i][j + 1] - 1);
                    right = rooms.at(rightIndex);
                }
                rooms[currentIndex]->load(up, right, down, left);
            }
        }
    }

    for (int i = 0; i < MAP_SIZE_Y; i++)
    {
        for (int j = 0; j < MAP_SIZE_X; j++)
        {
            if (i == 4 && j == 3)
                map[i][j] = 2;
            else if (map[i][j] == n)
                map[i][j] = 3;
            else if (map[i][j] > 0)
                map[i][j] = 1;
        }
    }

    player.load();
    gui.load(map, player);
    currentRoom->start();
}

bool Level::handleInput(const sf::Event &event, const bool &controls, const sf::RenderWindow &window)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouseEvent->button == sf::Mouse::Button::Right && !controls && !moved)
        {
            target = Utils::mapToLogical(sf::Vector2f(mouseEvent->position), window);
            return false;
        }

        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            playerFire(Utils::mapToLogical(sf::Vector2f(mouseEvent->position), window));
            return false;
        }
    }

    else if (event.is<sf::Event::KeyPressed>())
    {
        const auto *keyPressed = event.getIf<sf::Event::KeyPressed>();

        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            return true;
        }

        else if (controls && !moved)
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::W)
                target = player.getPosition() + sf::Vector2f(0.f, -10.f);

            else if (keyPressed->scancode == sf::Keyboard::Scancode::A)
                target = player.getPosition() + sf::Vector2f(-10.f, 0.f);

            else if (keyPressed->scancode == sf::Keyboard::Scancode::S)
                target = player.getPosition() + sf::Vector2f(0.f, 10.f);

            else if (keyPressed->scancode == sf::Keyboard::Scancode::D)
                target = player.getPosition() + sf::Vector2f(10.f, 0.f);
        }
    }

    else if (event.is<sf::Event::MouseWheelScrolled>())
    {
        const auto *mouseWheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>();
        if (mouseWheelScrolled->delta > 0)
            player.cycleWeapon(true);
        else
            player.cycleWeapon(false);
    }

    return false;
}

void Level::handleMovementInput(const bool &controls, const sf::RenderWindow &window)
{
    moved = false;
    sf::Vector2f movement = player.getPosition();
    if (controls)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            movement += sf::Vector2f(0.f, -10.f);
            target = movement;
            moved = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            movement += sf::Vector2f(-10.f, 0.f);
            target = movement;
            moved = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            movement += sf::Vector2f(0.f, 10.f);
            target = movement;
            moved = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            movement += sf::Vector2f(10.f, 0.f);
            target = movement;
            moved = true;
        }
    }
    else
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        {
            movement = Utils::mapToLogical(sf::Vector2f(sf::Mouse::getPosition(window)), window);
            target = movement;
            moved = true;
        }
    }
}

void Level::handleShootInput(const sf::RenderWindow &window)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        playerFire(Utils::mapToLogical(sf::Vector2f(sf::Mouse::getPosition(window)), window));
}

void Level::draw(sf::RenderWindow &window)
{
    currentRoom->draw(window);
    gui.draw(window);
}

int Level::update(const float &dt, const sf::Vector2f &mousePosition)
{
    if (player.getHealthStatus().x <= 0)
        return 1; // Defeat

    int movedRooms = -1;
    sf::Vector2f oldPosition = player.getPosition();

    player.update(dt, target, mousePosition);

    if (!currentRoom)
        throw GameStateException("currentRoom is null in Level::update");

    std::pair<int, std::weak_ptr<Room>> action = currentRoom->update(dt);
    
    if (currentRoom == rooms.back() && currentRoom->isCleared())
        return 2; // Victory
    
    if (action.first == -2)
    {
        sf::Vector2f newPosition = player.getPosition();

        player.setPosition(sf::Vector2f(newPosition.x, oldPosition.y), mousePosition);
        int testActionX = currentRoom->checkPlayerCollisions();

        player.setPosition(sf::Vector2f(oldPosition.x, newPosition.y), mousePosition);
        int testActionY = currentRoom->checkPlayerCollisions();

        if (testActionX == -2 && testActionY == -2)
            player.setPosition(oldPosition, mousePosition);

        else if (testActionX == -2)
            player.setPosition(sf::Vector2f(oldPosition.x, newPosition.y), mousePosition);

        else if (testActionY == -2)
            player.setPosition(sf::Vector2f(newPosition.x, oldPosition.y), mousePosition);

        else
            player.setPosition(oldPosition, mousePosition);
    }

    else if (!action.second.expired())
    {
        currentRoom = action.second.lock();

        if (action.first == 0)
        {
            player.setPosition({LOGICAL_WIDTH * 0.5f, LOGICAL_HEIGHT - 195.f}, mousePosition);
            movedRooms = 0;
        }

        else if (action.first == 1)
        {
            player.setPosition({195.f, LOGICAL_HEIGHT * 0.5f}, mousePosition);
            movedRooms = 1;
        }

        else if (action.first == 2)
        {
            player.setPosition({LOGICAL_WIDTH * 0.5f, 195.f}, mousePosition);
            movedRooms = 2;
        }

        else if (action.first == 3)
        {
            player.setPosition({LOGICAL_WIDTH - 195.f, LOGICAL_HEIGHT * 0.5f}, mousePosition);
            movedRooms = 3;
        }
        target = player.getPosition();
        currentRoom->start();
    }

    gui.update(movedRooms);
    
    return 0;
}

void Level::playerFire(const sf::Vector2f &fireTarget)
{
    currentRoom->spawnPlayerProjectile(fireTarget);
}

std::ostream &operator<<(std::ostream &os, const Level &level)
{
    os << "Level:\n";
    os << level.player << "\n";
    os << level.gui << "\n\n";
    if (level.currentRoom)
        os << "Current Room: " << *level.currentRoom << "\n";
    else
        os << "Current Room: None\n";
    return os;
}