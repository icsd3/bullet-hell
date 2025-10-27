#include "../headers/GameLogic.h"
#include <iostream>

Game::Game()
    :window(sf::VideoMode::getDesktopMode(), "BulletHell", sf::Style::Default, sf::State::Fullscreen)
{
    setup();
}

Game& Game::getInstance()
{
    static Game instance;
    return instance;
}

void Game::setup()
{
    Loader.loadStaticAssets();
    currentState = menu;
    handleNewState();
    window.setVerticalSyncEnabled(true);
}

void Game::handleNewState()
{
    switch (currentState)
    {
        case menu:
            Loader.loadMainMenu(window);
            std::cout << "menu\n";
            break;
        case augment_1:
            Loader.loadAugment(window);
            std::cout << "augment_1\n";
            break;
        case augment_2:
            Loader.loadAugment(window);
            std::cout << "augment_2\n";
            break;
        case augment_3:
            Loader.loadAugment(window);
            std::cout << "augment_3\n";
            break;
        case level_1:
            Loader.loadLevel(window);
            player.position = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);
            target = player.position;
            player.loadPlayer();
            std::cout << "level_1\n";
            break;
        case level_2:
            Loader.loadLevel(window);
            std::cout << "level_2\n";
            break;
        case level_3:
            Loader.loadLevel(window);
            std::cout << "level_3\n";
            break;
        case defeat:
            Loader.loadDefeat(window);;
            std::cout << "defeat\n";
            break;
        case victory:
            Loader.loadVictory(window);
            std::cout << "victory\n";
            break;
        default:
            break;
    }
}

bool Game::handleInputs()
{
    bool shouldExit = false;

        while(const std::optional event = window.pollEvent()) 
        {
            if (event->is<sf::Event::Closed>()) 
            {
                window.close();
                std::cout << "Fereastra a fost inchisa fortat\n";
            }
            else if (event->is<sf::Event::KeyPressed>()) 
            {
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                if(keyPressed->scancode == sf::Keyboard::Scancode::Enter) 
                {
                    if (currentState == menu)
                        handleMainMenuInput(*event);
                }
                if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) 
                {
                    shouldExit=true;
                    std::cout<<"Escape pressed bye bye!\n";
                }
            }
            else if (event->is<sf::Event::MouseButtonPressed>())
            {
                const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseEvent->button == sf::Mouse::Button::Left)
                {
                    if (currentState == menu)
                        shouldExit = handleMainMenuInput(*event);
                    else if(currentState == augment_1 || currentState == augment_2 || currentState == augment_3)
                    {
                        handleAugmentInput(*event);
                    }
                }
                else if (mouseEvent->button == sf::Mouse::Button::Right)
                {
                    if(currentState == level_1 || currentState == level_2 || currentState == level_3)
                    {
                        handleLevelInput(*event);
                    }
                }
            }
        }
        return shouldExit;
}

bool Game::handleMainMenuInput(const sf::Event& event)
{
    if(event.is<sf::Event::MouseButtonPressed>())
    {
        if (!Loader.menuButtonSprite[0] || !Loader.menuButtonSprite[1]) 
            return false;

        auto mousePos = sf::Mouse::getPosition(window);
        const auto startButtonBounds = Loader.menuButtonSprite[0]->getGlobalBounds();
        const auto exitButtonBounds = Loader.menuButtonSprite[1]->getGlobalBounds();

        if (startButtonBounds.contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))) 
        {
            std::cout << "Start button clicked!\n";
            Progress::selectGameState(currentState);
            handleNewState();
        }
        else if(exitButtonBounds.contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))))
        {
            std::cout << "Exit button clicked!\n";
            return true;
        }
    }
    else
    {
        std::cout << "Enter key pressed!\n";
        Progress::selectGameState(currentState);
        handleNewState();
    }
    return false;
}

void Game::handleAugmentInput(const sf::Event& event)
{
    if(event.is<sf::Event::MouseButtonPressed>())
    {
        if (!Loader.augmentButtonSprite[0] || !Loader.augmentButtonSprite[1] || !Loader.augmentButtonSprite[2]) 
            return;

        auto mousePos = sf::Mouse::getPosition(window);
        for(int i=0; i<3; i++)
        {
            const auto buttonBounds = Loader.augmentButtonSprite[i]->getGlobalBounds();

            if (buttonBounds.contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))) 
            {
                std::cout << "Augment button " << i+1 << " clicked!\n";
                Progress::selectGameState(currentState);
                handleNewState();
                break;
            }
        }
    }
}

void Game::handleLevelInput(const sf::Event& event)
{
    if(event.is<sf::Event::MouseButtonPressed>())
    {
        const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

        if (!player.sprite) 
            return;

        if(mouseEvent->button == sf::Mouse::Button::Right)
        {
            target = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        }
    }
}

void Game::drawMenu()
{
    if(Loader.menuBackgroundSprite) 
        window.draw(*Loader.menuBackgroundSprite);
    if(Loader.menuButtonSprite[0])
        window.draw(*Loader.menuButtonSprite[0]);
    if(Loader.menuButtonSprite[1])
        window.draw(*Loader.menuButtonSprite[1]);
}

void Game::drawAugment()
{
    if(Loader.augmentBackgroundSprite) 
        window.draw(*Loader.augmentBackgroundSprite);
    for(int i=0; i<3; i++)
        if(Loader.augmentButtonSprite[i])
            window.draw(*(Loader.augmentButtonSprite[i]));
}

void Game::drawLevel()
{
    if(Loader.levelBackgroundSprite) 
        window.draw(*Loader.levelBackgroundSprite);
    if(player.sprite)
        window.draw(*player.sprite);
}

void Game::drawDefeat()
{

}

void Game::drawVictory()
{
    
}

void Game::draw()
{
    window.clear();
        
        switch (currentState)
        {
            case menu:
                drawMenu();
                break;

            case augment_1:
            case augment_2:
            case augment_3:
                drawAugment();
                break;

            case level_1:
            case level_2:
            case level_3:
                drawLevel();
                break;
            
            case defeat:
                drawDefeat();
                break;

            case victory:
                drawVictory();
                break;

            default:
                break;
        }

    window.display();
}

void Game::Play()
{
    while(window.isOpen()) 
    {
        bool shouldExit = handleInputs();

        if(shouldExit) 
        {
            window.close();
            std::cout << "Fereastra a fost inchisa (shouldExit == true)\n";
            break;
        }
        if(currentState == level_1 || currentState == level_2 || currentState == level_3)
            player.updatePlayer(clock, target);
        draw();
    }
}

std::ostream& operator<<(std::ostream& os, const Game& game)
{
    os << "Current Game State: " << game.currentState << "\n";
    return os;
}