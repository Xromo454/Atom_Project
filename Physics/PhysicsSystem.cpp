#include <cmath>
#include <algorithm>
#include <SFML/System/Vector2.hpp>

#include "PhysicsSystem.hpp"
#include "Core/Entities/Atom.hpp"

sf::Vector2f PhysicsSystem::computePairForce(const Atom &a, const Atom &b)
{
    if (!a.element || !b.element)
        return sf::Vector2f(0.0f, 0.0f);

    sf::Vector2f delta = b.pos - a.pos;
    float dist2 = delta.x * delta.x + delta.y * delta.y;

    const float minDistance = 2.0f;
    if (dist2 < minDistance * minDistance)
    {
        dist2 = minDistance * minDistance;
    }

    float dist = std::sqrt(dist2);
    sf::Vector2f direction = delta / dist;

    float eps = std::sqrt(a.element->epsilon * b.element->epsilon);
    float sig = (a.element->sigma + b.element->sigma) * 0.5f * 1.5f;

    float sigOverR = sig / dist;
    float sigOverR2 = sigOverR * sigOverR;
    float sigOverR6 = sigOverR2 * sigOverR2 * sigOverR2;
    float sigOverR12 = sigOverR6 * sigOverR6;

    float forceMagnitude = (24.0f * eps / dist) * (2.0f * sigOverR12 - sigOverR6);

    const float maxForce = 150000.0f;
    forceMagnitude = std::clamp(forceMagnitude, -maxForce, maxForce);

    return direction * (-forceMagnitude);
}

void PhysicsSystem::applyInteractions(std::vector<Atom> &atoms)
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