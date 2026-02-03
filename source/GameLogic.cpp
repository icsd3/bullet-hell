#include "../headers/GameLogic.h"
#include "../headers/SoundManager.h"

Game::Game()
    : currentState(main_menu),
      player(std::make_unique<Player>()),
      gui(std::make_unique<GUI>()),
      level(std::make_unique<Level>(*player, *gui)),
      window(sf::VideoMode::getDesktopMode(), "BulletHell", sf::Style::Default, sf::State::Fullscreen)
{
    setup();
}

void Game::setup()
{
    window.clear(sf::Color::Black);
    window.display();
    handleNewState();
    window.setFramerateLimit(144);
}

void Game::handleNewState()
{
    switch (currentState)
    {
    case main_menu:
        menu.load();
        settings.load();
        break;

    case combat:
        updateClock.restart();
        level->load(1);
        pauseMenu.load();
        inventory.load();
        break;

    case defeat:
        gameOverMenu.load(false);
        break;

    case victory:
        gameOverMenu.load(true);
        break;

    default:
        throw GameStateException("Unknown game state encountered in handleNewState");
    }
}

void Game::togglePause()
{
    if (Utils::changePaused(1))
    {
        updateClock.stop();
        SoundManager::pauseMusic();
    }
    else
    {
        updateClock.start();
        SoundManager::resumeMusic();
    }
}

void Game::resetGame()
{
    player = std::make_unique<Player>();
    gui = std::make_unique<GUI>();
    level = std::make_unique<Level>(*player, *gui);
    updateClock.restart();
    SoundManager::stopMusic();
}

bool Game::handleInputs()
{
    bool shouldExit = false;

    if ((currentState == combat) && !Utils::changePaused(0))
    {
        level->handleMovementInput(controls, window);
        level->handleShootInput(window);
    }

    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        else if ((currentState == defeat || currentState == victory) && event->is<sf::Event::MouseButtonPressed>())
        {
            int action = gameOverMenu.handleInput(window, *event);
            if (action == 1) // Main Menu
            {
                resetGame();
                currentState = main_menu;
                handleNewState();
            }
        }

        else if (event->is<sf::Event::FocusLost>())
        {
            if (currentState == combat)
            {
                if (!Utils::changePaused(0))
                {
                    togglePause();
                    openPauseMenu = true;
                }
            }
        }

        else if (event->is<sf::Event::Resized>())
        {
            const auto *resized = event->getIf<sf::Event::Resized>();
            sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(resized->size));
            window.setView(sf::View(visibleArea));
        }

        else if (event->is<sf::Event::KeyPressed>() || event->is<sf::Event::MouseButtonPressed>() || event->is<sf::Event::MouseWheelScrolled>())
        {
            if (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Backspace)
                shouldExit = true;

            else if (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::I)
                std::cout << *this;

            else if (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::H)
                Utils::changeDisplayBoxes(1);

            else if (openSettings)
            {
                int action = settings.handleInput(window, *event);

                switch (action)
                {
                case 1:
                    window.close();

                    if (fullscreen)
                        window.create(sf::VideoMode::getDesktopMode(), "BulletHell", sf::Style::Default, sf::State::Windowed);

                    else
                        window.create(sf::VideoMode::getDesktopMode(), "BulletHell", sf::Style::Default, sf::State::Fullscreen);

                    fullscreen = !fullscreen;
                    window.setFramerateLimit(currentFramerate);
                    window.clear(sf::Color::Black);
                    window.display();
                    break;

                case 2:
                    controls = !controls;
                    break;

                case 3:
                    openSettings = false;

                    if (!openPauseMenu && Utils::changePaused(0))
                        togglePause();

                    break;

                case 4:
                    if (currentFramerate == 60)
                        currentFramerate = 75;
                    else if (currentFramerate == 75)
                        currentFramerate = 90;
                    else if (currentFramerate == 90)
                        currentFramerate = 120;
                    else if (currentFramerate == 120)
                        currentFramerate = 144;
                    else
                        currentFramerate = 60;
                    window.setFramerateLimit(currentFramerate);
                    break;

                default:
                    break;
                }
            }

            else if (currentState == main_menu)
            {
                int action = menu.handleInput(window, *event);

                switch (action)
                {
                case 1:
                    currentState = combat;
                    handleNewState();
                    break;

                case 2:
                    openSettings = true;
                    break;

                case 3:
                    shouldExit = true;
                    break;

                default:
                    break;
                }
            }

            else if (openPauseMenu)
            {
                int action = pauseMenu.handleInput(window, *event);

                switch (action)
                {
                case 1: // Resume
                    openPauseMenu = false;
                    togglePause();
                    break;

                case 2: // Settings
                    openSettings = true;
                    break;

                case 3: // Main Menu
                    openPauseMenu = false;
                    togglePause();
                    resetGame();
                    currentState = main_menu;
                    break;
                }
            }

            else if ((currentState == combat) && !Utils::changePaused(0))
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Tab) && event->is<sf::Event::KeyPressed>())
                {
                    const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
                    if (keyEvent->scancode == sf::Keyboard::Scancode::Right)
                    {
                         inventory.changePage(true, player->getWeapons().size());
                    }
                    else if (keyEvent->scancode == sf::Keyboard::Scancode::Left)
                    {
                         inventory.changePage(false, player->getWeapons().size());
                    }
                }

                if (level->handleInput(*event, controls, window))
                {
                    togglePause();
                    openPauseMenu = true;
                }
            }
        }
    }
    return shouldExit;
}

void Game::draw()
{
    window.clear();

    switch (currentState)
    {
    case main_menu:
        menu.draw(window);
        break;

    case combat:
        level->draw(window);
        break;

    case defeat:
    case victory:
        level->draw(window);
        gameOverMenu.draw(window);
        break;

    default:
        break;
    }

    if (openPauseMenu)
    {
        pauseMenu.draw(window);
    }
    if (currentState == combat && !openPauseMenu && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Tab))
    {
        inventory.draw(window, player->getWeapons());
    }
    if (openSettings)
    {
        settings.draw(window);
    }

    window.display();
}

void Game::Play()
{
    while (window.isOpen())
    {
        bool shouldExit = handleInputs();

        if (shouldExit)
        {
            window.close();
            break;
        }

        if (currentState == combat)
        {
            float dt = 0;
            if (!Utils::changePaused(0))
                dt = updateClock.restart().asSeconds();
            int status = level->update(dt, Utils::mapToLogical(sf::Vector2f(sf::Mouse::getPosition(window)), window));
            
            if (status == 1)
            {
                currentState = defeat;
                handleNewState();
            }
            else if (status == 2)
            {
                currentState = victory;
                handleNewState();
            }
        }
        draw();
    }
}

std::ostream &operator<<(std::ostream &os, const Game &game)
{
    os << "\n####################################################################################################################################\n";
    os << game.settings << "\n";
    os << *game.level << "\n";
    os << "Window size: " << game.window.getSize().x << "x" << game.window.getSize().y << "\n\n";
    os << "Current State: " << game.currentState << "\n\n";
    os << "####################################################################################################################################\n\n";
    return os;
}