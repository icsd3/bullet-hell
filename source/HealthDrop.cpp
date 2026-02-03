#include "../headers/HealthDrop.h"

HealthDrop::HealthDrop(const sf::Vector2f &pos)
    : Drop(pos, ResourceManager::getTexture(TextureType::Heart))
{
    std::mt19937 &rng = Utils::getRng();
    std::uniform_int_distribution<int> healDist(1, 3);
    healAmount = healDist(rng) * 5;
}

bool HealthDrop::doApplyEffect(Player &player)
{
    if (player.getHealthStatus().x < player.getHealthStatus().y)
    {
        player.heal(healAmount);
        SoundManager::playSound(SoundType::PickupDrop, 50.f);
        return true;
    }
    return false;
}
