#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "../Element.hpp"

enum class UIAction
{
    None,
    SelectElement,
    TogglePause,
    ClearAll
};

class UIManager
{
private:
    float panelWidth;
    size_t selectedElementIndex;
    bool isPaused;

    sf::Font font;
    sf::RectangleShape panel;
    sf::Text titleText;
    sf::Text statsText;

    std::vector<sf::RectangleShape> elementButtons;
    std::vector<sf::Text> elementLabels;

    sf::RectangleShape pauseButton;
    sf::Text pauseLabel;

    sf::RectangleShape clearButton;
    sf::Text clearLabel;

public:
    UIManager(float width, float windowHeight);

    UIAction handleMouseClick(sf::Vector2f mousePos);
    void updateStats(size_t atomCount, int fps);
    void render(sf::RenderWindow& window);

    float getPanelWidth() const { return panelWidth; }
    size_t getSelectedElementIndex() const { return selectedElementIndex; }
    const ElementType& getSelectedElement() const { return ELEMENTS[selectedElementIndex]; }
    bool getIsPaused() const { return isPaused; }
    const sf::Font& getFont() const { return font; }
};