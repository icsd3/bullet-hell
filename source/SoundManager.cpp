#include "../headers/SoundManager.h"

std::vector<sf::Sound> SoundManager::soundPool;
size_t SoundManager::nextSoundIndex = 0;
sf::Music SoundManager::music;
bool SoundManager::musicLoaded = false;

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

void SoundManager::playMusic(SoundType type, float volume)
{
    try
    {
        music.stop();
        
        if (type == SoundType::LevelMusic)
        {
            if (!music.openFromFile("audio/music.wav"))
                throw ConfigurationException("Failed to load level music file");
        }
        else
        {
            throw ConfigurationException("Invalid music type");
        }
        
        music.setVolume(volume);
        music.setLooping(true);
        music.play();
        musicLoaded = true;
    }
    catch (const std::exception& e)
    {
        throw ConfigurationException("Failed to play music");
    }
}

void SoundManager::pauseMusic()
{
    if (musicLoaded && music.getStatus() == sf::SoundSource::Status::Playing)
    {
        music.pause();
    }
}

void SoundManager::resumeMusic()
{
    if (musicLoaded && music.getStatus() == sf::SoundSource::Status::Paused)
    {
        music.play();
    }
}

void SoundManager::stopMusic()
{
    if (musicLoaded)
    {
        music.stop();
        musicLoaded = false;
    }
}
