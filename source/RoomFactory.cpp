#include "../headers/RoomFactory.h"
#include "../headers/EnemyRoom.h"
#include "../headers/BossRoom.h"
#include "../headers/Exception.h"

std::shared_ptr<Room> RoomFactory::createRoom(RoomType type, Player& player, int nrOfEnemies) {
    switch (type) {
    case RoomType::Start:
        return std::make_shared<Room>(player);
    case RoomType::Enemy:
        return std::make_shared<EnemyRoom>(nrOfEnemies, player);
    case RoomType::Boss:
        return std::make_shared<BossRoom>(player);
    default:
        throw ConfigurationException("Invalid RoomType requested in RoomFactory");
    }
}
