#include <sfml/Graphics.hpp>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>

#include "Core/Entities/Atom.hpp"
#include "Core/Entities/Element.hpp"
#include "UI/UIManager.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Core/Bounds.hpp"
#include "Chemistry/ChemistrySystem.hpp"
#include "Core/Simulation.hpp"
int main()
{
    PhysicsSystem physicsSystem;
    ChemistrySystem chemistrySystem;
    Simulation simulation;
    Bounds windowBounds = {0.0f, 1100.0f, 0.0f, 650.0f};
    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned int>(windowBounds.right),
                       static_cast<unsigned int>(windowBounds.bottom)}),
        "Atom Simulation");
    window.setFramerateLimit(60);

    UIManager uiManager(240.0f, static_cast<float>(window.getSize().y));
    std::vector<Atom> &atoms = simulation.getAtoms();
    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        if (dt <= 0.0f)
            dt = 0.0166f;

        if (dt > 0.033f)
            dt = 0.033f;

        int fps = static_cast<int>(1.0f / dt);

        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto *mouseClick = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseClick->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(mouseClick->position);

                    UIAction action = uiManager.handleMouseClick(mousePos);

                    if (action == UIAction::ClearAll)
                    {
                        simulation.clear();
                    }
                    else if (mousePos.x > uiManager.getPanelWidth())
                    {
                        const ElementType &selectedElement = uiManager.getSelectedElement();
                        simulation.addAtom(mousePos, &selectedElement);
                    }
                }
            }
        }

        Bounds simulationBounds = {uiManager.getPanelWidth(), windowBounds.right, windowBounds.top, windowBounds.bottom};

        for (auto &atom : atoms)
        {
            atom.resetForce();
        }

        physicsSystem.applyInteractions(atoms);
        chemistrySystem.updateBonds(simulation);
        chemistrySystem.applyBondForces(simulation);
        for (auto &atom : atoms)
        {
            atom.update(dt, simulationBounds);
        }

        window.clear(sf::Color(15, 15, 22));

        for (const auto &atom : atoms)
        {
            for (const auto &bond : atom.bonds)
            {
                if (atom.id >= bond.neighborId)
                    continue;

                const Atom *neighbor = simulation.findAtomById(bond.neighborId);

                if (neighbor == nullptr)
                    continue;

                sf::Vector2f dir = neighbor->pos - atom.pos;
                float length = std::hypot(dir.x, dir.y);
                if (length < 0.001f)
                    continue;
                sf::Vector2f unitDir = dir / length;

                sf::Vector2f normal(-unitDir.y, unitDir.x);

                float spacing = 4.0f;

                if (bond.order == 1)
                {
                    sf::Vertex line[] = 
                        {{atom.pos, sf::Color::White},
                        {neighbor->pos, sf::Color::White}};
                    window.draw(line, 2, sf::PrimitiveType::Lines);
                }
                else if (bond.order == 2)
                {
                    sf::Vector2f offset = normal * spacing;

                    sf::Vertex line1[] = 
                        {{atom.pos + offset, sf::Color::White},
                        {neighbor->pos + offset, sf::Color::White}};
                    sf::Vertex line2[] = 
                        {{atom.pos - offset, sf::Color::White},
                        {neighbor->pos - offset, sf::Color::White}};

                    window.draw(line1, 2, sf::PrimitiveType::Lines);
                    window.draw(line2, 2, sf::PrimitiveType::Lines);
                }
                else if (bond.order == 3)
                {
                    sf::Vector2f offset = normal * (spacing * 1.4f);

                    sf::Vertex line1[] = 
                        {{atom.pos + offset, sf::Color::White},
                        {neighbor->pos + offset, sf::Color::White}};
                    sf::Vertex line2[] = 
                        {{atom.pos, sf::Color::White},
                        {neighbor->pos, sf::Color::White}};
                    sf::Vertex line3[] = 
                        {{atom.pos - offset, sf::Color::White},
                        {neighbor->pos - offset, sf::Color::White}};

                    window.draw(line1, 2, sf::PrimitiveType::Lines);
                    window.draw(line2, 2, sf::PrimitiveType::Lines);
                    window.draw(line3, 2, sf::PrimitiveType::Lines);
                }
            }
        }

        for (const auto &atom : atoms)
        {
            sf::CircleShape shape(atom.radius(), 32);
            shape.setOrigin(sf::Vector2f(atom.radius(), atom.radius()));
            shape.setPosition(atom.pos);
            shape.setFillColor(atom.color());
            window.draw(shape);

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