#include "../headers/Settings.h"

Settings::Settings()
    : screenText(ResourceManager::getFont(FontType::Courier)), controlsText(ResourceManager::getFont(FontType::Courier)), framerateText(ResourceManager::getFont(FontType::Courier))
{
}

Settings &Settings::getInstance()
{
    static Settings instance;
    return instance;
}

void Settings::load()
{
    settingsBox.setSize(sf::Vector2f(0.4f * LOGICAL_WIDTH, 0.4f * LOGICAL_HEIGHT));
    settingsBox.setOrigin(sf::Vector2f(0.5f * settingsBox.getLocalBounds().size.x, 0.5f * settingsBox.getLocalBounds().size.y));
    settingsBox.setPosition(sf::Vector2f(0.5f * LOGICAL_WIDTH, 0.5f * LOGICAL_HEIGHT));
    settingsBox.setFillColor(sf::Color(0, 0, 0, 150));
    settingsBox.setOutlineThickness(10.f);
    settingsBox.setOutlineColor(sf::Color::Black);

    screenButton.setSize(sf::Vector2f(0.3f * LOGICAL_WIDTH, 0.05f * LOGICAL_HEIGHT));
    screenButton.setOrigin(sf::Vector2f(screenButton.getLocalBounds().position.x + 0.5f * screenButton.getLocalBounds().size.x,
                                        screenButton.getLocalBounds().position.y + 0.5f * screenButton.getLocalBounds().size.y));
    screenButton.setPosition(sf::Vector2f(settingsBox.getPosition().x, settingsBox.getPosition().y - 100.f));
    screenButton.setFillColor(sf::Color::Black);
    screenButton.setOutlineThickness(3.f);
    screenButton.setOutlineColor(sf::Color::White);

    controlsButton.setSize(sf::Vector2f(0.3f * LOGICAL_WIDTH, 0.05f * LOGICAL_HEIGHT));
    controlsButton.setOrigin(sf::Vector2f(controlsButton.getLocalBounds().position.x + 0.5f * controlsButton.getLocalBounds().size.x,
                                          controlsButton.getLocalBounds().position.y + 0.5f * controlsButton.getLocalBounds().size.y));
    controlsButton.setPosition(sf::Vector2f(settingsBox.getPosition().x, settingsBox.getPosition().y));
    controlsButton.setFillColor(sf::Color::Black);
    controlsButton.setOutlineThickness(3.f);
    controlsButton.setOutlineColor(sf::Color::White);

    framerateButton.setSize(sf::Vector2f(0.3f * LOGICAL_WIDTH, 0.05f * LOGICAL_HEIGHT));
    framerateButton.setOrigin(sf::Vector2f(framerateButton.getLocalBounds().position.x + 0.5f * framerateButton.getLocalBounds().size.x,
                                          framerateButton.getLocalBounds().position.y + 0.5f * framerateButton.getLocalBounds().size.y));
    framerateButton.setPosition(sf::Vector2f(settingsBox.getPosition().x, settingsBox.getPosition().y + 100.f));
    framerateButton.setFillColor(sf::Color::Black);
    framerateButton.setOutlineThickness(3.f);
    framerateButton.setOutlineColor(sf::Color::White);

    screenText.setCharacterSize(30);
    screenText.setFillColor(sf::Color::White);
    screenText.setString("Window Mode: Fullscreen");
    screenText.setOrigin(sf::Vector2f(screenText.getLocalBounds().position.x + 0.5f * screenText.getLocalBounds().size.x,
                                      screenText.getLocalBounds().position.y + 0.5f * screenText.getLocalBounds().size.y));
    screenText.setPosition(screenButton.getPosition());

    controlsText.setCharacterSize(30);
    controlsText.setFillColor(sf::Color::White);
    controlsText.setString("Controls: WASD");
    controlsText.setOrigin(sf::Vector2f(controlsText.getLocalBounds().position.x + 0.5f * controlsText.getLocalBounds().size.x,
                                        controlsText.getLocalBounds().position.y + 0.5f * controlsText.getLocalBounds().size.y));
    controlsText.setPosition(controlsButton.getPosition());

    framerateText.setCharacterSize(30);
    framerateText.setFillColor(sf::Color::White);
    framerateText.setString("Framerate: 144");
    framerateText.setOrigin(sf::Vector2f(framerateText.getLocalBounds().position.x + 0.5f * framerateText.getLocalBounds().size.x,
                                        framerateText.getLocalBounds().position.y + 0.5f * framerateText.getLocalBounds().size.y));
    framerateText.setPosition(framerateButton.getPosition());
}

void Settings::draw(sf::RenderWindow &window)
{
    sf::Vector2f scaleFactor = Utils::getScaleFactor(window);

    sf::RectangleShape drawBox = settingsBox;
    drawBox.setPosition(Utils::mapToScreen(settingsBox.getPosition(), window));
    drawBox.setSize(sf::Vector2f(settingsBox.getSize().x * scaleFactor.x, settingsBox.getSize().y * scaleFactor.y));
    drawBox.setOrigin(sf::Vector2f(drawBox.getSize().x / 2.f, drawBox.getSize().y / 2.f));
    drawBox.setOutlineThickness(settingsBox.getOutlineThickness() * scaleFactor.x);
    window.draw(drawBox);

    sf::RectangleShape drawScreenButton = screenButton;
    drawScreenButton.setPosition(Utils::mapToScreen(screenButton.getPosition(), window));
    drawScreenButton.setSize(sf::Vector2f(screenButton.getSize().x * scaleFactor.x, screenButton.getSize().y * scaleFactor.y));
    drawScreenButton.setOrigin(sf::Vector2f(drawScreenButton.getSize().x / 2.f, drawScreenButton.getSize().y / 2.f));
    drawScreenButton.setOutlineThickness(screenButton.getOutlineThickness() * scaleFactor.x);
    window.draw(drawScreenButton);

    sf::RectangleShape drawControlsButton = controlsButton;
    drawControlsButton.setPosition(Utils::mapToScreen(controlsButton.getPosition(), window));
    drawControlsButton.setSize(sf::Vector2f(controlsButton.getSize().x * scaleFactor.x, controlsButton.getSize().y * scaleFactor.y));
    drawControlsButton.setOrigin(sf::Vector2f(drawControlsButton.getSize().x / 2.f, drawControlsButton.getSize().y / 2.f));
    drawControlsButton.setOutlineThickness(controlsButton.getOutlineThickness() * scaleFactor.x);
    window.draw(drawControlsButton);

    sf::RectangleShape drawFramerateButton = framerateButton;
    drawFramerateButton.setPosition(Utils::mapToScreen(framerateButton.getPosition(), window));
    drawFramerateButton.setSize(sf::Vector2f(framerateButton.getSize().x * scaleFactor.x, framerateButton.getSize().y * scaleFactor.y));
    drawFramerateButton.setOrigin(sf::Vector2f(drawFramerateButton.getSize().x / 2.f, drawFramerateButton.getSize().y / 2.f));
    drawFramerateButton.setOutlineThickness(framerateButton.getOutlineThickness() * scaleFactor.x);
    window.draw(drawFramerateButton);

    sf::Text drawScreenText = screenText;
    drawScreenText.setPosition(Utils::mapToScreen(screenText.getPosition(), window));
    drawScreenText.setOrigin(sf::Vector2f(drawScreenText.getLocalBounds().position.x + 0.5f * drawScreenText.getLocalBounds().size.x,
                                          drawScreenText.getLocalBounds().position.y + 0.5f * drawScreenText.getLocalBounds().size.y));
    drawScreenText.setScale(scaleFactor);
    window.draw(drawScreenText);

    sf::Text drawControlsText = controlsText;
    drawControlsText.setPosition(Utils::mapToScreen(controlsText.getPosition(), window));
    drawControlsText.setOrigin(sf::Vector2f(drawControlsText.getLocalBounds().position.x + 0.5f * drawControlsText.getLocalBounds().size.x,
                                            drawControlsText.getLocalBounds().position.y + 0.5f * drawControlsText.getLocalBounds().size.y));
    drawControlsText.setScale(scaleFactor);
    window.draw(drawControlsText);

    sf::Text drawFramerateText = framerateText;
    drawFramerateText.setPosition(Utils::mapToScreen(framerateText.getPosition(), window));
    drawFramerateText.setOrigin(sf::Vector2f(drawFramerateText.getLocalBounds().position.x + 0.5f * drawFramerateText.getLocalBounds().size.x,
                                            drawFramerateText.getLocalBounds().position.y + 0.5f * drawFramerateText.getLocalBounds().size.y));
    drawFramerateText.setScale(scaleFactor);
    window.draw(drawFramerateText);
}

int Settings::handleInput(const sf::RenderWindow &window, const sf::Event &event)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos = Utils::mapToLogical(sf::Vector2f(mouseEvent->position), window);

            const auto screenButtonBounds = screenButton.getGlobalBounds();
            const auto controlsButtonBounds = controlsButton.getGlobalBounds();
            const auto framerateButtonBounds = framerateButton.getGlobalBounds();

            if (screenButtonBounds.contains(mousePos))
            {
                if (screenText.getString() == "Window Mode: Fullscreen")
                {
                    screenText.setString("Window Mode: Windowed");
                }
                else
                {
                    screenText.setString("Window Mode: Fullscreen");
                }
                return 1; // Change window mode
            }
            if (controlsButtonBounds.contains(mousePos))
            {
                if (controlsText.getString() == "Controls: WASD")
                {
                    controlsText.setString("Controls: Mouse");
                }
                else
                {
                    controlsText.setString("Controls: WASD");
                }
                return 2; // Change controls
            }
            if (framerateButtonBounds.contains(mousePos))
            {
                if (framerateText.getString() == "Framerate: 60")
                    framerateText.setString("Framerate: 75");
                else if (framerateText.getString() == "Framerate: 75")
                    framerateText.setString("Framerate: 90");
                else if (framerateText.getString() == "Framerate: 90")
                    framerateText.setString("Framerate: 120");
                else if (framerateText.getString() == "Framerate: 120")
                    framerateText.setString("Framerate: 144");
                else
                    framerateText.setString("Framerate: 60");
                return 4; // Change framerate
            }
        }
    }
    else if (event.is<sf::Event::KeyPressed>())
    {
        const auto *keyboardEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyboardEvent->scancode == sf::Keyboard::Scancode::Escape)
        {
            return 3; // Close settings
        }
    }
    return 0;
}

std::ostream &operator<<(std::ostream &os, const Settings &settings)
{
    os << "Settings:\n";
    os << "    " << std::string(settings.screenText.getString()) << "\n";
    os << "    " << std::string(settings.controlsText.getString()) << "\n";
    os << "    " << std::string(settings.framerateText.getString()) << "\n";
    return os;
}