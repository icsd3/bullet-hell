#include "../headers/Level.h"

Level::Level(Player &p, GUI &g)
    : player(p), gui(g), map{{0}}
{
}

void Level::generateRooms(const int n)
{
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

            if (nx < 0 || ny < 0 || nx >= 5 || ny >= 7)
                continue;

            if (map[nx][ny])
                continue;

            int count = 0;

            for (const auto &i : dirs)
            {
                int mx = nx + i.first;
                int my = ny + i.second;
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
    bool visited[5][7] = {{false}};

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
            room = std::make_shared<Room>(player);
        else if (rooms.size() < static_cast<size_t>(n - 1))
        {
            std::uniform_int_distribution<int> nrOfEnemiesDist(4, 6);
            room = std::make_shared<EnemyRoom>(nrOfEnemiesDist(rng), player);
        }
        else
            room = std::make_shared<BossRoom>(player);

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
    currentRoom = rooms[0];

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            std::weak_ptr<Room> up;
            std::weak_ptr<Room> down;
            std::weak_ptr<Room> left;
            std::weak_ptr<Room> right;
            if (map[i][j] > 0)
            {
                if (i - 1 >= 0 && map[i - 1][j] > 0)
                    up = rooms[map[i - 1][j] - 1];
                if (i + 1 < 5 && map[i + 1][j] > 0)
                    down = rooms[map[i + 1][j] - 1];
                if (j - 1 >= 0 && map[i][j - 1] > 0)
                    left = rooms[map[i][j - 1] - 1];
                if (j + 1 < 7 && map[i][j + 1] > 0)
                    right = rooms[map[i][j + 1] - 1];
                rooms[map[i][j] - 1]->load(up, right, down, left);
            }
        }
    }

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 7; j++)
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

std::pair<int, sf::Vector2f> Level::handleInput(const sf::Event &event, const bool &controls, const sf::RenderWindow &window)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouseEvent->button == sf::Mouse::Button::Right && !controls)
        {
            return std::make_pair(1, Utils::mapToLogical(sf::Vector2f(mouseEvent->position), window));
        }

        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            return std::make_pair(2, Utils::mapToLogical(sf::Vector2f(mouseEvent->position), window));
        }
    }
    if (event.is<sf::Event::KeyPressed>())
    {
        const auto *keyPressed = event.getIf<sf::Event::KeyPressed>();

        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            return std::make_pair(3, sf::Vector2f(-1.f, -1.f));
        }

        else if (controls)
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::W)
                return std::make_pair(1, player.getPosition() + sf::Vector2f(0.f, -10.f));

            else if (keyPressed->scancode == sf::Keyboard::Scancode::A)
                return std::make_pair(1, player.getPosition() + sf::Vector2f(-10.f, 0.f));

            else if (keyPressed->scancode == sf::Keyboard::Scancode::S)
                return std::make_pair(1, player.getPosition() + sf::Vector2f(0.f, 10.f));

            else if (keyPressed->scancode == sf::Keyboard::Scancode::D)
                return std::make_pair(1, player.getPosition() + sf::Vector2f(10.f, 0.f));
        }
    }

    return std::make_pair(0, sf::Vector2f(-1.f, -1.f));
}

sf::Vector2f Level::handleMovementInput(const bool &controls, const sf::RenderWindow &window)
{
    if (controls)
    {
        sf::Vector2f movement = player.getPosition();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            movement += sf::Vector2f(0.f, -10.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            movement += sf::Vector2f(-10.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            movement += sf::Vector2f(0.f, 10.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            movement += sf::Vector2f(10.f, 0.f);

        return movement;
    }
    else
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        {
            return Utils::mapToLogical(sf::Vector2f(sf::Mouse::getPosition(window)), window);
        }
    }
    return {-1.f, -1.f};
}

sf::Vector2f Level::handleShootInput(const sf::RenderWindow &window)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        return Utils::mapToLogical(sf::Vector2f(sf::Mouse::getPosition(window)), window);
    }
    return {-1.f, -1.f};
}

void Level::draw(sf::RenderWindow &window)
{
    currentRoom->draw(window);
    gui.draw(window);
}

void Level::update(const float &dt, sf::Vector2f &target)
{
    int moved = -1;
    sf::Vector2f oldPosition = player.getPosition();

    player.update(dt, target);

    std::pair<int, std::weak_ptr<Room>> action = currentRoom->update(dt);

    if (action.first == -2)
    {
        sf::Vector2f newPosition = player.getPosition();

        player.setPosition(sf::Vector2f(newPosition.x, oldPosition.y));
        int testActionX = currentRoom->checkPlayerCollisions();

        player.setPosition(sf::Vector2f(oldPosition.x, newPosition.y));
        int testActionY = currentRoom->checkPlayerCollisions();

        if (testActionX == -2 && testActionY == -2)
            player.setPosition(oldPosition);

        else if (testActionX == -2)
            player.setPosition(sf::Vector2f(oldPosition.x, newPosition.y));

        else if (testActionY == -2)
            player.setPosition(sf::Vector2f(newPosition.x, oldPosition.y));

        else
            player.setPosition(oldPosition);
    }

    else if (!action.second.expired())
    {
        currentRoom = action.second.lock();

        if (action.first == 0)
        {
            player.setPosition({LOGICAL_WIDTH * 0.5f, LOGICAL_HEIGHT - 195.f});
            moved = 0;
        }

        else if (action.first == 1)
        {
            player.setPosition({195.f, LOGICAL_HEIGHT * 0.5f});
            moved = 1;
        }

        else if (action.first == 2)
        {
            player.setPosition({LOGICAL_WIDTH * 0.5f, 195.f});
            moved = 2;
        }

        else if (action.first == 3)
        {
            player.setPosition({LOGICAL_WIDTH - 195.f, LOGICAL_HEIGHT * 0.5f});
            moved = 3;
        }
        target = player.getPosition();
        currentRoom->start();
    }

    gui.update(moved);
}

void Level::playerFire(const sf::Vector2f &target)
{
    currentRoom->spawnPlayerProjectile(target);
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