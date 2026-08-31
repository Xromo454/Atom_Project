#include <cmath>
#include <SFML/System/Vector2.hpp>

#include "PhysicsSystem.hpp"
#include "../Atom.hpp"

sf::Vector2f PhysicsSystem::computePairForce(const Atom& a, const Atom& b)
{
    sf::Vector2f delta = b.pos - a.pos;

    float dx = delta.x;
    float dy = delta.y;
    float r2 = dx * dx + dy * dy;

    if (r2 < 0.0001f) // Avoid division by zero
        return sf::Vector2f(0.0f, 0.0f);
    
    float dist = std::sqrt(r2);
    sf::Vector2f direction = delta / dist;

    float desiredDistance = (a.radius() + b.radius()) * 1.5f;
    
    const float strength = 200.0f;
    float forceMagnitude = strength * (dist - desiredDistance);

    return direction * forceMagnitude;
}

void PhysicsSystem::applyInteractions(std::vector<Atom>& atoms)
{
    for (size_t i = 0; i < atoms.size(); ++i)
    {
        for (size_t j = i + 1; j < atoms.size(); ++j)
        {
            sf::Vector2f force = computePairForce(atoms[i], atoms[j]);
            atoms[i].applyForce(force);
            atoms[j].applyForce(-force);
        }
    }
}