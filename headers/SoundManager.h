#pragma once

#include "../headers/ResourceManager.h"

#include <SFML/Audio.hpp>
#include <vector>

class SoundManager
{
    static constexpr int MAX_SOUNDS = 32;
    static std::vector<sf::Sound> soundPool;
    static size_t nextSoundIndex;

public:
    static void playSound(SoundType type, float volume = 100.f);
};
