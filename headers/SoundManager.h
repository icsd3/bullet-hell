#pragma once

#include "../headers/ResourceManager.h"

#include <SFML/Audio.hpp>
#include <vector>

class SoundManager
{
    static constexpr int MAX_SOUNDS = 32;
    static std::vector<sf::Sound> soundPool;
    static size_t nextSoundIndex;
    
    static sf::Music music;
    static bool musicLoaded;

public:
    static void playSound(SoundType type, float volume = 100.f);
    
    static void playMusic(SoundType type, float volume = 100.f);
    static void pauseMusic();
    static void resumeMusic();
    static void stopMusic();
};
