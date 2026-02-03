#pragma once

#include "Singleton.h"
#include "Weapon.h"

class Inventory : public Singleton<Inventory>
{
    sf::RectangleShape background;
    sf::Text titleText;
    
    int currentPage = 0;
    const int itemsPerPage = 6;
    
    void drawWeaponEntry(sf::RenderWindow &window, const Weapon &weapon, const sf::Vector2f &position, const sf::Vector2f &scaleFactor);

protected:
    Inventory();

public:
    void load();
    void draw(sf::RenderWindow &window, const std::vector<std::unique_ptr<Weapon>> &weapons);
    
    void changePage(bool next, int totalItems);
};
