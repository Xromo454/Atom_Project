#include "UIManager.hpp"
#include <iostream>
#include <string>

namespace
{
    sf::Font loadFont(const std::string &path)
    {
        sf::Font font;
        if (!font.openFromFile(path))
        {
            std::cerr << "Warning: font not loaded from " << path << "!\n";
        }
        return font;
    }
}

UIManager::UIManager(float width, float windowHeight)
    : panelWidth(width),
      selectedElementIndex(0),
      isPaused(false),
      font(loadFont("UI/arialmt.ttf")),
      panel(sf::Vector2f(panelWidth, windowHeight)),
      titleText(font, "Select Element:", 18),
      statsText(font, "", 14),
      pauseLabel(font, "Pause", 16),
      clearLabel(font, "Clear All", 16)
{

    panel.setFillColor(sf::Color(25, 28, 36));

    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(sf::Vector2f(20.0f, 15.0f));

    statsText.setFillColor(sf::Color::White);
    statsText.setPosition(sf::Vector2f(20.0f, 600.0f));

    for (size_t i = 0; i < ELEMENTS.size(); ++i)
    {
        sf::RectangleShape btn(sf::Vector2f(200.0f, 35.0f));
        btn.setPosition(sf::Vector2f(20.0f, 50.0f + static_cast<float>(i) * 45.0f));
        btn.setFillColor(i == 0 ? sf::Color(60, 120, 200) : sf::Color(45, 50, 65));
        elementButtons.push_back(btn);

        sf::Text label(font, ELEMENTS[i].name, 15);
        label.setFillColor(sf::Color::White);
        label.setPosition(sf::Vector2f(30.0f, 57.0f + static_cast<float>(i) * 45.0f));
        elementLabels.push_back(label);
    }

    pauseButton.setSize({200.0f, 35.0f});
    pauseButton.setPosition(sf::Vector2f(20.0f, 500.0f));
    pauseButton.setFillColor(sf::Color(180, 120, 30));
    pauseLabel.setFillColor(sf::Color::White);
    pauseLabel.setPosition(sf::Vector2f(95.0f, 507.0f));

    clearButton.setSize({200.0f, 35.0f});
    clearButton.setPosition(sf::Vector2f(20.0f, 550.0f));
    clearButton.setFillColor(sf::Color(180, 50, 50));
    clearLabel.setFillColor(sf::Color::White);
    clearLabel.setPosition(sf::Vector2f(85.0f, 557.0f));
}

UIAction UIManager::handleMouseClick(sf::Vector2f mousePos)
{
    if (mousePos.x > panelWidth)
        return UIAction::None;

    for (size_t i = 0; i < elementButtons.size(); ++i)
    {
        if (elementButtons[i].getGlobalBounds().contains(mousePos))
        {
            selectedElementIndex = i;

            for (size_t j = 0; j < elementButtons.size(); ++j)
            {
                elementButtons[j].setFillColor(
                    j == i ? sf::Color(60, 120, 200) : sf::Color(45, 50, 65));
            }

            return UIAction::SelectElement;
        }
    }

    if (pauseButton.getGlobalBounds().contains(mousePos))
    {
        isPaused = !isPaused;
        pauseLabel.setString(isPaused ? "Resume" : "Pause");
        return UIAction::TogglePause;
    }

    if (clearButton.getGlobalBounds().contains(mousePos))
    {
        return UIAction::ClearAll;
    }

    return UIAction::None;
}

void UIManager::updateStats(size_t atomCount, int fps)
{
    statsText.setString("Atoms: " + std::to_string(atomCount) + " | FPS: " + std::to_string(fps));
}

void UIManager::render(sf::RenderWindow &window)
{
    window.draw(panel);
    window.draw(titleText);

    for (size_t i = 0; i < elementButtons.size(); ++i)
    {
        window.draw(elementButtons[i]);
        window.draw(elementLabels[i]);
    }

    window.draw(pauseButton);
    window.draw(pauseLabel);
    window.draw(clearButton);
    window.draw(clearLabel);
    window.draw(statsText);
}