#include "../headers/Exception.h"

GeneralException::GeneralException(const std::string& msg) 
    : message(msg) 
{
}

const char* GeneralException::what() const noexcept 
{ 
    return message.c_str(); 
}

ResourceLoadException::ResourceLoadException(const std::string& msg)
    : GeneralException("Load Error: " + msg) 
{
}

TextureLoadException::TextureLoadException(const std::string& msg)
    : ResourceLoadException("Texture: " + msg) 
{
}

FontLoadException::FontLoadException(const std::string& msg)
    : ResourceLoadException("Font: " + msg) 
{
}

AudioLoadException::AudioLoadException(const std::string& msg)
    : ResourceLoadException("Audio: " + msg) 
{
}

ResourceFetchException::ResourceFetchException(const std::string& msg)
    : GeneralException("Fetch Error: " + msg) 
{
}

TextureFetchException::TextureFetchException(const std::string& msg)
    : ResourceFetchException("Texture: " + msg) 
{
}

FontFetchException::FontFetchException(const std::string& msg)
    : ResourceFetchException("Font: " + msg) 
{
}

AudioFetchException::AudioFetchException(const std::string& msg)
    : ResourceFetchException("Audio: " + msg) 
{
}

GameStateException::GameStateException(const std::string& msg)
    : GeneralException("Game State Error: " + msg) 
{
}

ConfigurationException::ConfigurationException(const std::string& msg)
    : GeneralException("Configuration Error: " + msg) 
{
}

OutOfBoundsException::OutOfBoundsException(const std::string& msg)
    : GeneralException("Out of Bounds Error: " + msg) 
{
}
