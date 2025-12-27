#include "headers/GameLogic.h"

int main() 
{
    try
    {
        Game& Game = Game::getInstance();
        Game.Play();
    }
    catch (const TextureLoadException& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (const TextureFetchException& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (const FontLoadException& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (const FontFetchException& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (const AudioLoadException& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (const AudioFetchException& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (const ConfigurationException& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (const GameStateException& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (const OutOfBoundsException& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}