#include "../headers/GameStates.h"
#include <iostream>

void Progress::selectGameState(gameStates &gameState)
{
    switch (gameState)
    {
    case menu:
        gameState = augment_1;
        break;

    case augment_1:
        gameState = level_1;
        break;

    case level_1:
        gameState = augment_2;
        break;

    case augment_2:
        gameState = level_2;
        break;

    case level_2:
        gameState = augment_3;
        break;

    case augment_3:
        gameState = level_3;
        break;

    case level_3:
        gameState = victory;
        break;

    case defeat:
    case victory:
    default:
        gameState = menu;
        break;
    }
}