#include "../headers/Settings.h"

Settings::Settings()
    : fullscreen(true), controls(true)
{
}

Settings &Settings::getInstance()
{
    static Settings instance;
    return instance;
}

bool Settings::changeFullscreen()
{
    fullscreen = !fullscreen;
    return fullscreen;
}

bool Settings::changeControls()
{
    controls = !controls;
    return controls;
}

std::ostream &operator<<(std::ostream &os, const Settings &settings)
{
    os << "    Fullscreen: " << (settings.fullscreen ? "Enabled" : "Disabled") << "\n";
    os << "    Controls: " << (settings.controls ? "Enabled" : "Disabled") << "\n";
    return os;
}