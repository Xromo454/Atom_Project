#include <sfml/Graphics.hpp>
#include <algorithm>
#include <vector>
#include <iostream>

#include "Core/Entities/Atom.hpp"
#include "Core/Entities/Element.hpp"
#include "UI/UIManager.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Core/Bounds.hpp"
#include "Chemistry/ChemistrySystem.hpp"
int main()
{
    PhysicsSystem physicsSystem;
    ChemistrySystem chemistrySystem;
    Bounds windowBounds = {0.0f, 1100.0f, 0.0f, 650.0f};
    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned int>(windowBounds.right),
                       static_cast<unsigned int>(windowBounds.bottom)}),
        "Atom Simulation");
    window.setFramerateLimit(60);

    UIManager uiManager(240.0f, static_cast<float>(window.getSize().y));
    std::vector<Atom> atoms;
    sf::Clock clock;
    AtomStableId nextAtomId = 0;

    while (window.isOpen())
    {
        // Calculate delta time and FPS
        float dt = clock.restart().asSeconds();
        if (dt <= 0.0f)
            dt = 0.0166f;

        // Ограничиваем dt, чтобы при просадках FPS атомы не «пролетали» сквозь стенки
        if (dt > 0.033f)
            dt = 0.033f;

        int fps = static_cast<int>(1.0f / dt);

        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // Mouse click handling
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
                        const ElementType &selectedElement = uiManager.getSelectedElement();
                        atoms.emplace_back(mousePos, &selectedElement);
                        atoms.back().id = nextAtomId++;
                    }
                }
            }
        }

        // --- 1. ФИЗИЧЕСКИЙ ЦИКЛ ОБНОВЛЕНИЯ ---
        Bounds simulationBounds = {uiManager.getPanelWidth(), windowBounds.right, windowBounds.top, windowBounds.bottom};

        // А. Сначала очищаем все силы от прошлого кадра
        for (auto &atom : atoms)
        {
            atom.resetForce();
        }

        // Б. Начисляем межатомные силы Ван-дер-Ваальса (Леннард-Джонс)
        physicsSystem.applyInteractions(atoms);

        for (auto &atom : atoms)
        {
            atom.update(dt, simulationBounds);
        }

        chemistrySystem.updateBonds(atoms);

        // После этого начинается отрисовка
        window.clear(sf::Color(15, 15, 22));

        // Draw each chemical bond once. Bonds are stored in both atoms.
        for (const auto &atom : atoms)
        {
            for (const auto &bond : atom.bonds)
            {
                if (atom.id >= bond.neighborId)
                    continue;

                auto neighborIt = std::find_if(
                    atoms.begin(),
                    atoms.end(),
                    [&bond](const Atom &candidate)
                    {
                        return candidate.id == bond.neighborId;
                    });

                if (neighborIt == atoms.end())
                    continue;

                sf::Vertex bondLine[] = {
                    {atom.pos, sf::Color::White},
                    {neighborIt->pos, sf::Color::White}};

                window.draw(bondLine, 2, sf::PrimitiveType::Lines);
            }
        }

        for (const auto &atom : atoms)
        {
            // Атом
            sf::CircleShape shape(atom.radius(), 32);
            shape.setOrigin(sf::Vector2f(atom.radius(), atom.radius()));
            shape.setPosition(atom.pos);
            shape.setFillColor(atom.color());
            window.draw(shape);

            // Текст элемента
            sf::Text label(uiManager.getFont(), atom.element->symbol, 14);
            label.setFillColor(sf::Color::Black);

            sf::FloatRect textBounds = label.getLocalBounds();
            label.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f,
                             textBounds.position.y + textBounds.size.y / 2.0f});

            label.setPosition(atom.pos);
            window.draw(label);
        }

        uiManager.updateStats(atoms.size(), fps);
        uiManager.render(window);
        window.display();
    }
    return 0;
}