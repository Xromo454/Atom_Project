#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Element.hpp"

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
    sf::RectangleShape uiPanel;
    sf::Text titleText;
    sf::Text statsText;

    std::vector<sf::RectangleShape> elemButtons;
    std::vector<sf::Text> elemTexts;

    sf::RectangleShape btnPause;
    sf::Text txtPause;

    sf::RectangleShape btnClear;
    sf::Text txtClear;

public:
    UIManager(float width, float windowHeight);

    // Обработка клика, возвращает тип действия
    UIAction handleMouseClick(sf::Vector2f mousePos);

    void updateStats(size_t atomCount, int fps);
    void render(sf::RenderWindow &window);

    float getPanelWidth() const { return panelWidth; }
    size_t getSelectedElementIndex() const { return selectedElementIndex; }
    const ElementType &getSelectedElement() const { return ELEMENTS[selectedElementIndex]; }
    bool getIsPaused() const { return isPaused; }
};