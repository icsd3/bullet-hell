#include "../headers/Inventory.h"
#include <iomanip>
#include <sstream>

Inventory::Inventory() 
    : titleText(ResourceManager::getFont(FontType::Courier))
{
}

void Inventory::load()
{
    background.setSize(sf::Vector2f(0.4f * LOGICAL_WIDTH, 0.8f * LOGICAL_HEIGHT));
    background.setOrigin(background.getSize() * 0.5f);
    background.setPosition(sf::Vector2f(LOGICAL_WIDTH * 0.5f, LOGICAL_HEIGHT * 0.5f));
    background.setFillColor(sf::Color(0, 0, 0, 150)); 
    background.setOutlineThickness(10.f);
    background.setOutlineColor(sf::Color::Black);

    titleText.setString("INVENTORY");
    titleText.setCharacterSize(40);
    titleText.setFillColor(sf::Color::White);
    sf::FloatRect bounds = titleText.getLocalBounds();
    titleText.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    titleText.setPosition(sf::Vector2f(LOGICAL_WIDTH * 0.5f, LOGICAL_HEIGHT * 0.15f));
}

void Inventory::draw(sf::RenderWindow &window, const std::vector<std::unique_ptr<Weapon>> &weapons)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    if (weapons.empty())
        currentPage = 0;
    else if (static_cast<size_t>(currentPage * itemsPerPage) >= weapons.size())
        currentPage = (int)((weapons.size() - 1) / itemsPerPage);

    sf::RectangleShape drawBg = background;
    drawBg.setPosition(Utils::mapToScreen(background.getPosition(), window));
    drawBg.setSize(sf::Vector2f(background.getSize().x * scaleFactor.x, background.getSize().y * scaleFactor.y));
    drawBg.setOrigin(drawBg.getSize() * 0.5f);
    drawBg.setOutlineThickness(background.getOutlineThickness() * scaleFactor.x);
    window.draw(drawBg);

    sf::Text drawTitle = titleText;
    
    if (!weapons.empty())
    {
        int totalPages = (weapons.size() - 1) / itemsPerPage + 1;
        std::stringstream ss;
        ss << "INVENTORY (" << (currentPage + 1) << "/" << totalPages << ")";
        drawTitle.setString(ss.str());
        sf::FloatRect bounds = drawTitle.getLocalBounds();
        drawTitle.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    }

    drawTitle.setPosition(Utils::mapToScreen(titleText.getPosition(), window));
    drawTitle.setScale(scaleFactor);
    window.draw(drawTitle);

    float startY = LOGICAL_HEIGHT * 0.25f;
    float startX = LOGICAL_WIDTH * 0.35f; 
    float paddingY = 120.f; 

    size_t start = currentPage * itemsPerPage;
    size_t end = std::min(weapons.size(), start + itemsPerPage);

    for (size_t i = start; i < end; ++i)
    {
        sf::Vector2f pos(startX, startY + (i - start) * paddingY);
        drawWeaponEntry(window, *weapons[i], pos, scaleFactor);
    }
}

void Inventory::drawWeaponEntry(sf::RenderWindow &window, const Weapon &weapon, const sf::Vector2f &position, const sf::Vector2f &scaleFactor)
{
    Weapon::WeaponStats stats = weapon.getStats();

    if (stats.texture)
    {
        sf::Sprite icon(*stats.texture);
        float iconSize = 80.f;
        float scale = iconSize / std::max(stats.texture->getSize().x, stats.texture->getSize().y);
        icon.setScale(sf::Vector2f(scale, scale));
        icon.setOrigin(sf::Vector2f(stats.texture->getSize().x / 2.f, stats.texture->getSize().y / 2.f));
        
        icon.setPosition(Utils::mapToScreen(position + sf::Vector2f(50.f, 40.f), window));
        icon.scale(scaleFactor);
        window.draw(icon);
    }

    sf::Text statsText(ResourceManager::getFont(FontType::Arial));
    statsText.setCharacterSize(24);
    statsText.setFillColor(sf::Color::White);

    std::stringstream ss;
    ss << stats.name << "\n"
       << "Damage: " << stats.damage << " | Speed: " << std::fixed << std::setprecision(2) << stats.attackSpeed 
       << " | Range: " << (int)stats.range;

    statsText.setString(ss.str());
    statsText.setPosition(Utils::mapToScreen(position + sf::Vector2f(120.f, 10.f), window));
    statsText.setScale(scaleFactor);
    window.draw(statsText);
}

void Inventory::changePage(bool next, int totalItems)
{
    if (totalItems == 0) 
    {
        currentPage = 0;
        return;
    }

    int maxPage = (totalItems - 1) / itemsPerPage;

    if (next)
    {
        currentPage++;
        if (currentPage > maxPage)
            currentPage = 0;
    }
    else
    {
        currentPage--;
        if (currentPage < 0)
            currentPage = maxPage;
    }
}
