#pragma once

#include "Room.h"
#include "Player.h"

enum class RoomType {
    Start,
    Enemy,
    Boss
};

class RoomFactory {
    Player& player;

public:
    explicit RoomFactory(Player& p);
    
    std::shared_ptr<Room> createRoom(RoomType type);
};
