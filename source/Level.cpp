#include "../headers/Level.h"

Level::Level()
    :playerTexturePath("textures/player.png"),
    enemyPath("textures/enemy.png"),
    playerProjectilePath("textures/player_projectile.png"),
    enemyProjectilePath("textures/enemy_projectile.png"),
    roomBackgroundPath("textures/room_background.png"),
    doorVerticalPath("textures/door_vertical.png"),
    doorHorizontalPath("textures/door_horizontal.png"),
    playerTexture(playerTexturePath),
    enemyTexture(enemyPath),
    playerProjectileTexture(playerProjectilePath),
    enemyProjectileTexture(enemyProjectilePath),
    roomBackgroundTexture(roomBackgroundPath),
    doorVerticalTexture(doorVerticalPath),
    doorHorizontalTexture(doorHorizontalPath),
    map{{0}}
{
}

Level &Level::getInstance() 
{
    static Level instance;
    return instance;
}

void Level::generateRooms(const int n)
{
    map[4][3] = 1;
    
    std::vector<std::pair<int,int>> frontier;
    frontier.emplace_back(4, 3);

    std::vector<std::pair<int,int>> dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

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

        std::vector<std::pair<int,int>> moves;

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

        std::uniform_int_distribution<int> distM(0, moves.size()-1);
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

        rooms.emplace_back(up, right, down, left, doorVerticalTexture, doorHorizontalTexture, roomBackgroundTexture);

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
    currentRoom = &rooms[0];

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            Room *up = nullptr;
            Room *down = nullptr;
            Room *left = nullptr;
            Room *right = nullptr;
            if (map[i][j] > 0)
            {
                if (i-1 >= 0 && map[i-1][j] > 0)
                    up = &rooms[map[i-1][j]-1];
                if (i+1 < 5 && map[i+1][j] > 0)
                    down = &rooms[map[i+1][j]-1];
                if (j-1 >= 0 && map[i][j-1] > 0)
                    left = &rooms[map[i][j-1]-1];
                if (j+1 < 7 && map[i][j+1] > 0)
                    right = &rooms[map[i][j+1]-1];
                rooms[map[i][j]-1].load(up, right, down, left);
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

    spawnEnemies(4);
    player.load();
    gui.load(map);
}

void Level::spawnEnemies(const int &nrOfEnemies)
{
    for (int i = 1; i <= nrOfEnemies; i++)
    {
        enemies.push_back(Enemy::spawnEnemy(enemyTexture, sf::Vector2f(300.f + i%2 * 1320.f, 300.f + (i-1) / 2 * 480.f), 100.f, 100, enemyProjectilePath, enemyProjectileTexture));
        enemies.back().load();
    }
}

std::pair<int, sf::Vector2f> Level::handleInput(const sf::Event &event, const bool& controls, const sf::RenderWindow &window)
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

        else if(controls)
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
    if(controls)
    {
        sf::Vector2f movement = player.getPosition();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
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

    for (auto &projectile : playerProjectiles)
        projectile.draw(window);

    for (auto &projectile : enemyProjectiles)
        projectile.draw(window);

    for (auto &enemy : enemies)
        enemy.draw(window);

    player.draw(window);
    gui.draw(window);
}

void Level::update(const float &dt, const sf::Vector2f &target)
{
    int moved = -1;
    sf::Vector2f oldPosition = player.getPosition();
    
    player.update(dt, target);
    bool finished = enemies.empty();

    std::pair<int, Room*> action = currentRoom->update(player, finished);

    if (action.first == -2)
    {
        sf::Vector2f newPosition = player.getPosition();
        
        player.setPosition(sf::Vector2f(newPosition.x, oldPosition.y));
        int testActionX = currentRoom->checkCollisions(player);
        
        player.setPosition(sf::Vector2f(oldPosition.x, newPosition.y));
        int testActionY = currentRoom->checkCollisions(player);
        
        if (testActionX == -2 && testActionY == -2)
            player.setPosition(oldPosition);

        else if (testActionX == -2)
            player.setPosition(sf::Vector2f(oldPosition.x, newPosition.y));

        else if (testActionY == -2)
            player.setPosition(sf::Vector2f(newPosition.x, oldPosition.y));

        else
            player.setPosition(oldPosition);
    }

    else if (action.second != nullptr)
    {
        currentRoom = action.second;
        playerProjectiles.clear();
        enemyProjectiles.clear();

        if (action.first == 0)
        {
            player.setPosition({LOGICAL_WIDTH * 0.5f, LOGICAL_HEIGHT * 0.8f});
            moved = 0;
        }

        else if (action.first == 1)
        {
            player.setPosition({LOGICAL_WIDTH * 0.1f, LOGICAL_HEIGHT * 0.5f});
            moved = 1;
        }

        else if (action.first == 2)
        {
            player.setPosition({LOGICAL_WIDTH * 0.5f, LOGICAL_HEIGHT * 0.15f});
            moved = 2;
        }

        else if (action.first == 3)
        {
            player.setPosition({LOGICAL_WIDTH * 0.9f, LOGICAL_HEIGHT * 0.5f});
            moved = 3;
        }
    }

    for (size_t i = 0; i < playerProjectiles.size();)
    {
        if (playerProjectiles[i].update(dt))
            playerProjectiles.erase(playerProjectiles.begin() + i);

        else
        {
            if (checkEnemyHits(playerProjectiles[i]))
                playerProjectiles.erase(playerProjectiles.begin() + i);

            else
                i++;
        }
    }

    sf::Vector2f enemyTarget = player.getPosition();

    for(auto &enemy : enemies)
    {
        std::vector<Projectile> bullets = enemy.update(enemyTarget);

        for(const auto &bullet : bullets)
        {
            enemyProjectiles.push_back(bullet);
            enemyProjectiles.back().load();
        }
    }

    for (size_t i = 0; i < enemyProjectiles.size();)
    {
        if (enemyProjectiles[i].update(dt))
            enemyProjectiles.erase(enemyProjectiles.begin() + i);

        else
        {
            if (checkPlayerHits(enemyProjectiles[i], player))
                enemyProjectiles.erase(enemyProjectiles.begin() + i);

            else
                i++;
        }
    }
    gui.update(player.getHealthStatus(), moved);
}

bool Level::checkEnemyHits(const Projectile &projectile)
{
    for (size_t i = 0; i < enemies.size();)
    {
        if (int damage = projectile.hits(enemies[i]))
        {
            if (enemies[i].takeDamage(damage))
                enemies.erase(enemies.begin() + i);
            return true;
        }

        else
            i++;
    }
    return false;
}

bool Level::checkPlayerHits(const Projectile &projectile, Player &player)
{
    if (int damage = projectile.hits(player))
    {
        if (player.takeDamage(damage))
            std::cout<<"game over\n";
        return true;
    }  
    return false;
}

void Level::spawnPlayerProjectile(const sf::Vector2f &target)
{
    std::vector<Projectile> bullets = player.fire(target, playerProjectileTexture);
            
    for(const auto &bullet : bullets)
    {
        playerProjectiles.push_back(bullet);
        playerProjectiles.back().load();
    }
}