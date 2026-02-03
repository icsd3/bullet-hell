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
    static bool sfxEnabled;
    static bool musicEnabled;

public:
    static void playSound(SoundType type, float volume = 50.f);
    
    static void playMusic(SoundType type, float volume = 50.f);
    static void pauseMusic();
    static void resumeMusic();
    static void stopMusic();
    
    static void setSFXEnabled(bool enabled);
    static void setMusicEnabled(bool enabled);
    static bool isSFXEnabled();
    static bool isMusicEnabled();
};
