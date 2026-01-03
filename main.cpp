#include <exception>
#include <iostream>
#include "headers/GameLogic.h"

int main() 
{
    try
    {
        Game& Game = Game::getInstance();
        Game.Play();
    }
    catch (const GeneralException& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown Exception\n";
    }
    return 0;
}