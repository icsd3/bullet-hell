#include "../headers/GUI.h"

GUI::GUI()
    : health(ResourceManager::getFont(FontType::Arial)), 
      weaponName(ResourceManager::getFont(FontType::Arial)),
      fps(ResourceManager::getFont(FontType::Arial))
{
}

std::ostream &operator<<(std::ostream &os, const GUI &gui)
{
    os << "    Health Text: " << gui.health.getString().toAnsiString() << "\n";
    os << "    FPS Text: " << gui.fps.getString().toAnsiString();
    return os;
}

void GUI::load(const int map[MAP_SIZE_Y][MAP_SIZE_X], Player &p)
{
    player = &p;
    maxHealthBar.setSize(sf::Vector2f(0.4f * LOGICAL_WIDTH, 44.f));
    maxHealthBar.setOrigin(sf::Vector2f(0.5f * maxHealthBar.getLocalBounds().size.x, 0.5f * maxHealthBar.getLocalBounds().size.y));
    maxHealthBar.setPosition(sf::Vector2f(0.5f * LOGICAL_WIDTH, LOGICAL_HEIGHT - 0.5f * maxHealthBar.getSize().y - 3.f));
    maxHealthBar.setFillColor(sf::Color(0, 50, 0, 175));
    maxHealthBar.setOutlineThickness(3.f);
    maxHealthBar.setOutlineColor(sf::Color::Black);

    currentHealthBar.setSize(maxHealthBar.getSize());
    currentHealthBar.setOrigin(sf::Vector2f(0, 0.5f * currentHealthBar.getLocalBounds().size.y));
    currentHealthBar.setPosition(sf::Vector2f(maxHealthBar.getPosition().x - 0.5f * maxHealthBar.getSize().x, maxHealthBar.getPosition().y));
    currentHealthBar.setFillColor(sf::Color(0, 100, 0, 175));

    health.setCharacterSize(30);
    health.setFillColor(sf::Color::Green);
    health.setOutlineThickness(2.f);
    health.setOutlineColor(sf::Color::Black);

    weaponNameBox.setSize(sf::Vector2f(maxHealthBar.getSize().x * 0.5f, maxHealthBar.getSize().y));
    weaponNameBox.setOrigin(sf::Vector2f(weaponNameBox.getSize().x * 0.5f, weaponNameBox.getSize().y * 0.5f));
    weaponNameBox.setPosition(sf::Vector2f(maxHealthBar.getPosition().x, maxHealthBar.getPosition().y - maxHealthBar.getSize().y * 0.5f - weaponNameBox.getSize().y * 0.5f));
    weaponNameBox.setFillColor(sf::Color(0, 0, 0, 150));
    weaponNameBox.setOutlineThickness(-3.f);
    weaponNameBox.setOutlineColor(sf::Color::Black);
    
    weaponName.setCharacterSize(30);
    weaponName.setFillColor(sf::Color::White);
    weaponName.setOutlineThickness(2.f);
    weaponName.setOutlineColor(sf::Color::Black);

    fps.setCharacterSize(20);
    fps.setFillColor(sf::Color::Green);
    fps.setPosition(sf::Vector2f(LOGICAL_WIDTH - 100.f, 10.f));

    minimap.load(map);
}

void GUI::update(const int &moved)
{
    sf::Vector2i HP = player->getHealthStatus();
    currentHealthBar.setSize(sf::Vector2f(static_cast<float>(HP.x) / static_cast<float>(HP.y) * maxHealthBar.getSize().x, currentHealthBar.getSize().y));

    health.setString("HP: " + std::to_string(HP.x) + " / " + std::to_string(HP.y));
    health.setOrigin(sf::Vector2f(health.getLocalBounds().position.x + 0.5f * health.getLocalBounds().size.x, health.getLocalBounds().position.y + 0.5f * health.getLocalBounds().size.y));
    health.setPosition(maxHealthBar.getPosition());

    weaponName.setString(player->getCurrentWeaponName());
    sf::FloatRect wBounds = weaponName.getLocalBounds();
    weaponName.setOrigin(sf::Vector2f(wBounds.position.x + wBounds.size.x * 0.5f, wBounds.position.y + wBounds.size.y * 0.5f));
    weaponName.setPosition(weaponNameBox.getPosition());

    if (fpsClock.getElapsedTime().asSeconds() >= 1.f)
    {
        fps.setString("FPS: " + std::to_string(frameCount));
        fps.setOrigin(sf::Vector2f(fps.getLocalBounds().position.x + fps.getLocalBounds().size.x, fps.getLocalBounds().position.y));
        fps.setPosition(sf::Vector2f(LOGICAL_WIDTH - 10.f, 10.f));
        frameCount = 0;
        fpsClock.restart();
    }
    else
    {
        frameCount++;
    }

    minimap.update(moved);
}

void GUI::draw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    sf::RectangleShape drawMaxHealthBar = maxHealthBar;
    drawMaxHealthBar.setPosition(Utils::mapToScreen(maxHealthBar.getPosition(), window));
    drawMaxHealthBar.setSize(sf::Vector2f(maxHealthBar.getSize().x * scaleFactor.x, maxHealthBar.getSize().y * scaleFactor.y));
    drawMaxHealthBar.setOrigin(sf::Vector2f(drawMaxHealthBar.getSize().x / 2.f, drawMaxHealthBar.getSize().y / 2.f));
    drawMaxHealthBar.setOutlineThickness(maxHealthBar.getOutlineThickness() * scaleFactor.x);
    window.draw(drawMaxHealthBar);

    sf::RectangleShape drawCurrentHealthBar = currentHealthBar;
    drawCurrentHealthBar.setPosition(Utils::mapToScreen(currentHealthBar.getPosition(), window));
    drawCurrentHealthBar.setSize(sf::Vector2f(currentHealthBar.getSize().x * scaleFactor.x, currentHealthBar.getSize().y * scaleFactor.y));
    drawCurrentHealthBar.setOrigin(sf::Vector2f(0, drawCurrentHealthBar.getSize().y / 2.f));
    window.draw(drawCurrentHealthBar);

    sf::Text drawHealth = health;
    drawHealth.setPosition(Utils::mapToScreen(health.getPosition(), window));
    drawHealth.setScale(scaleFactor);
    window.draw(drawHealth);

    sf::RectangleShape drawWeaponBox = weaponNameBox;
    drawWeaponBox.setPosition(Utils::mapToScreen(weaponNameBox.getPosition(), window));
    drawWeaponBox.setSize(sf::Vector2f(weaponNameBox.getSize().x * scaleFactor.x, weaponNameBox.getSize().y * scaleFactor.y));
    drawWeaponBox.setOrigin(sf::Vector2f(drawWeaponBox.getSize().x * 0.5f, drawWeaponBox.getSize().y * 0.5f));
    drawWeaponBox.setOutlineThickness(weaponNameBox.getOutlineThickness() * scaleFactor.x);
    window.draw(drawWeaponBox);

    sf::Text drawWeaponName = weaponName;
    drawWeaponName.setPosition(Utils::mapToScreen(weaponName.getPosition(), window));
    drawWeaponName.setScale(scaleFactor);
    window.draw(drawWeaponName);

    sf::Text drawFps = fps;
    drawFps.setPosition(Utils::mapToScreen(fps.getPosition(), window));
    drawFps.setScale(scaleFactor);
    window.draw(drawFps);

    minimap.draw(window);
}