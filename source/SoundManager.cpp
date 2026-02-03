#include "../headers/SoundManager.h"

std::vector<sf::Sound> SoundManager::soundPool;
size_t SoundManager::nextSoundIndex = 0;

void SoundManager::playSound(SoundType type, float volume)
{
    try
    {
        sf::SoundBuffer& buffer = ResourceManager::getSound(type);
        
        if (soundPool.size() < MAX_SOUNDS)
        {
            soundPool.emplace_back(buffer);
            soundPool.back().setVolume(volume);
            soundPool.back().play();
        }
        else
        {
            sf::Sound& sound = soundPool[nextSoundIndex];
            nextSoundIndex = (nextSoundIndex + 1) % MAX_SOUNDS;
            
            sound.setBuffer(buffer);
            sound.setVolume(volume);
            sound.play();
        }
    }
    catch (const std::exception& e)
    {
        throw ConfigurationException("Failed to play sound");
    }
}
