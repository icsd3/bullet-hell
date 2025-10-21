#include <iostream>
#include <array>
#include <chrono>
#include <thread>

#include "headers/GameLogic.h"

//////////////////////////////////////////////////////////////////////
/// This class is used to test that the memory leak checks work as expected even when using a GUI
class SomeClass {
public:
    explicit SomeClass(int) {}
};

SomeClass *getC() {
    return new SomeClass{2};
}
//////////////////////////////////////////////////////////////////////


int main() {
    std::cout << "Hello, world!\n";

    SomeClass *c = getC();
    std::cout << c << "\n";
    delete c;  // comentarea acestui rând ar trebui să ducă la semnalarea unui mem leak

    Game& Game = Game::getInstance();
    Game.Play();

    std::cout << "Programul a terminat execuția\n";
    return 0;
}
