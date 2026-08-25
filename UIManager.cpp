#include "UIManager.hpp"
#include <iostream>

UIManager::UIManager(float width, float windowHeight)
    : panelWidth(width),
      selectedElementIndex(0),
      isPaused(false),
      titleText(font, "Select Element:", 18),
      statsText(font, "", 14),
      txtPause(font, "Pause", 16),
      txtClear(font, "Clear All", 16)
{
    if (!font.openFromFile("arialmt.ttf"))
    {
        std::cerr << "Error loading font in UIManager!" << std::endl;
    }

    // Фон панели
    uiPanel.setSize({panelWidth, windowHeight});
    uiPanel.setFillColor(sf::Color(25, 28, 36));

    // Позиции текстов
    titleText.setPosition({20.0f, 15.0f});
    txtPause.setPosition({95.0f, 507.0f});
    txtClear.setPosition({85.0f, 557.0f});
    statsText.setPosition({20.0f, 600.0f});

    // Кнопки элементов
    for (size_t i = 0; i < ELEMENTS.size(); ++i)
    {
        sf::RectangleShape btn({200.0f, 35.0f});
        btn.setPosition({20.0f, 50.0f + i * 45.0f});
        btn.setFillColor(i == 0 ? sf::Color(60, 120, 200) : sf::Color(45, 50, 65));
        elemButtons.push_back(btn);

        sf::Text txt(font, ELEMENTS[i].name, 15);
        txt.setPosition({30.0f, 57.0f + i * 45.0f});
        elemTexts.push_back(txt);
    }

    // Кнопка Пауза
    btnPause.setSize({200.0f, 35.0f});
    btnPause.setPosition({20.0f, 500.0f});
    btnPause.setFillColor(sf::Color(180, 120, 30));

    // Кнопка Сброс
    btnClear.setSize({200.0f, 35.0f});
    btnClear.setPosition({20.0f, 550.0f});
    btnClear.setFillColor(sf::Color(180, 50, 50));
}

UIAction UIManager::handleMouseClick(sf::Vector2f mousePos)
{
    if (mousePos.x > panelWidth)
        return UIAction::None;

    for (size_t i = 0; i < elemButtons.size(); ++i)
    {
        if (elemButtons[i].getGlobalBounds().contains(mousePos))
        {
            selectedElementIndex = i;
            for (size_t j = 0; j < elemButtons.size(); ++j)
            {
                elemButtons[j].setFillColor(j == i ? sf::Color(60, 120, 200) : sf::Color(45, 50, 65));
            }
            return UIAction::SelectElement;
        }
    }

    if (btnPause.getGlobalBounds().contains(mousePos))
    {
        isPaused = !isPaused;
        txtPause.setString(isPaused ? "Resume" : "Pause");
        return UIAction::TogglePause;
    }

    if (btnClear.getGlobalBounds().contains(mousePos))
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
    window.draw(uiPanel);
    window.draw(titleText);

    for (size_t i = 0; i < elemButtons.size(); ++i)
    {
        window.draw(elemButtons[i]);
        window.draw(elemTexts[i]);
    }

    window.draw(btnPause);
    window.draw(txtPause);
    window.draw(btnClear);
    window.draw(txtClear);
    window.draw(statsText);
}