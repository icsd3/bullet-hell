#pragma once

#include <exception>
#include <string>

class GeneralException : public std::exception 
{
protected:
    std::string message;

public:
    explicit GeneralException(const std::string&);
    const char* what() const noexcept override;
};

// Resource loading

class ResourceLoadException : public GeneralException 
{
public:
    explicit ResourceLoadException(const std::string&);
};

class TextureLoadException : public ResourceLoadException 
{
public:
    explicit TextureLoadException(const std::string&);
};

class FontLoadException : public ResourceLoadException 
{
public:
    explicit FontLoadException(const std::string&);
};

class AudioLoadException : public ResourceLoadException 
{
public:
    explicit AudioLoadException(const std::string&);
};

// Resource fetching

class ResourceFetchException : public GeneralException 
{
public:
    explicit ResourceFetchException(const std::string&);
};

class TextureFetchException : public ResourceFetchException 
{
public:
    explicit TextureFetchException(const std::string&);
};

class FontFetchException : public ResourceFetchException 
{
public:
    explicit FontFetchException(const std::string&);
};

class AudioFetchException : public ResourceFetchException 
{
public:
    explicit AudioFetchException(const std::string&);
};

// Logic and config

class GameStateException : public GeneralException 
{
public:
    explicit GameStateException(const std::string&);
};

class ConfigurationException : public GeneralException 
{
public:
    explicit ConfigurationException(const std::string&);
};

class OutOfBoundsException : public GeneralException 
{
public:
    explicit OutOfBoundsException(const std::string&);
};