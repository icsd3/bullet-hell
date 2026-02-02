#pragma once

#include "Room.h"
#include "Player.h"

enum class RoomType {
    Start,
    Enemy,
    Boss
};

class RoomFactory {
public:
    static std::shared_ptr<Room> createRoom(RoomType type, Player& player, int nrOfEnemies = 0);
};
