#include "../headers/RoomFactory.h"
#include "../headers/EnemyRoom.h"
#include "../headers/BossRoom.h"
#include "../headers/Exception.h"
#include "../headers/Utils.h"

RoomFactory::RoomFactory(Player& p) 
    : player(p) 
{
}

std::shared_ptr<Room> RoomFactory::createRoom(RoomType type) {
    switch (type) 
    {
    case RoomType::Start:
        return std::make_shared<Room>(player);
        
    case RoomType::Enemy:
        {
            std::mt19937& rng = Utils::getRng();
            std::uniform_int_distribution<int> nrOfEnemiesDist(4, 6);
            int nrOfEnemies = nrOfEnemiesDist(rng);
            return std::make_shared<EnemyRoom>(nrOfEnemies, player);
        }
        
    case RoomType::Boss:
        return std::make_shared<BossRoom>(player);
        
    default:
        throw ConfigurationException("Invalid RoomType requested in RoomFactory");
    }
}
