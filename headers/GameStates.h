#pragma once

enum gameStates
{
    menu = 1,
    augment_1 = 2,
    level_1 = 3,
    augment_2 = 4,
    level_2 = 5,
    augment_3 = 6,
    level_3 = 7,
    defeat = 8,
    victory = 9
};

class Progress
{
    Progress() = default;

public:
    static void selectGameState(gameStates& gameState);
};