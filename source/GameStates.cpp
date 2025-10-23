#include "../headers/GameStates.h"
#include <iostream>

void Progress::selectGameState(int& gameState)
{
    switch (gameState)
    {
        case menu:
            gameState = augment_1;
            std::cout << "Transition: menu → augment_1\n";
            break;

        case augment_1:
            gameState = level_1;
            std::cout << "Transition: augment_1 → level_1\n";
            break;

        case level_1:
            gameState = augment_2;
            std::cout << "Transition: level_1 → augment_2\n";
            break;

        case augment_2:
            gameState = level_2;
            std::cout << "Transition: augment_2 → level_2\n";
            break;

        case level_2:
            gameState = augment_3;
            std::cout << "Transition: level_2 → augment_3\n";
            break;

        case augment_3:
            gameState = level_3;
            std::cout << "Transition: augment_3 → level_3\n";
            break;

        case level_3:
            gameState = victory;
            std::cout << "Transition: level_3 → victory\n";
            break;

        case game_over:
        case victory:
        default:
            gameState = menu;
            std::cout << "Transition: reset to menu\n";
            break;
    }
}
