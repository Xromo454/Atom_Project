#include <sfml/Graphics.hpp>
#include <vector>

#include "Atom.hpp"
#include "Element.hpp"
#include "UI/UIManager.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Core/Bounds.hpp"

int main()
{
    PhysicsSystem physicsSystem;
    Bounds windowBounds = {0.0f, 1100.0f, 0.0f, 650.0f};
    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned int>(windowBounds.right),
                       static_cast<unsigned int>(windowBounds.bottom)}),
        "Atom Simulation");
    window.setFramerateLimit(60);

    UIManager uiManager(240.0f, static_cast<float>(window.getSize().y));
    std::vector<Atom> atoms;
    sf::Clock clock;

    while (window.isOpen())
    {
        // Calculate delta time and FPS
        float dt = clock.restart().asSeconds();
        if (dt <= 0.0f)
            dt = 0.0166f;
        int fps = static_cast<int>(1.0f / dt);

        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            // mouse click handling
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
                    }
                }
            }
        }

        window.clear(sf::Color(15, 15, 22));

        // render atoms
        Bounds simulationBounds = {uiManager.getPanelWidth(), windowBounds.right, windowBounds.top, windowBounds.bottom};
        physicsSystem.applyInteractions(atoms);
        for (auto &atom : atoms)
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

            atom.applyForce(sf::Vector2f(0.0f, 98.1f * atom.mass()));
            atom.update(dt, simulationBounds);
            atom.resetForce();
        }

        uiManager.updateStats(atoms.size(), fps);
        uiManager.render(window);
        window.display();
    }
    return 0;
}