#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "Atom.hpp"
#include "UIManager.hpp"

int main()
{
    const float windowWidth = 1100;
    const float windowHeight = 650;
    const float UIWidth = 240.0f;

    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned int>(windowWidth), static_cast<unsigned int>(windowHeight)}),
        "Atom Simulation");
    window.setFramerateLimit(60);

    UIManager uiManager(UIWidth, windowHeight);

    std::vector<Atom> atoms;
    sf::CircleShape atomShape;
    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        if (dt > 0.05f)
            dt = 0.05f;

        // 1. ОБРАБОТКА СОБЫТИЙ
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto *mouseClick = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseClick->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(mouseClick->position);

                    UIAction action = uiManager.handleMouseClick(mousePos);

                    if (action == UIAction::ClearAll)
                    {
                        atoms.clear();
                    }
                    else if (mousePos.x > uiManager.getPanelWidth())
                    {
                        const ElementType &selectedType = uiManager.getSelectedElement();
                        Atom newAtom(mousePos);
                        newAtom.radius = selectedType.radius;
                        atoms.push_back(newAtom);
                    }
                }
            }
        }

        // 2. ОБНОВЛЕНИЕ СТАТИСТИКИ
        uiManager.updateStats(atoms.size(), static_cast<int>(1.0f / (dt > 0.0f ? dt : 0.0001f)));

        // 3. ОТРИСОВКА (Clear -> Draw -> Display)
        window.clear(sf::Color(15, 15, 22));

        for (auto &atom : atoms)
        {
            atomShape.setRadius(atom.radius);
            atomShape.setOrigin({atom.radius, atom.radius});
            atomShape.setPosition(atom.pos);
            atomShape.setFillColor(uiManager.getSelectedElement().color);
            window.draw(atomShape);
        }

        uiManager.render(window);
        window.display();
    }

    return 0;
}