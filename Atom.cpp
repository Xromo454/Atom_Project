#include "Atom.hpp"
#include "Core/Bounds.hpp"
Atom::Atom(sf::Vector2f position, const ElementType *elementType)
    : pos(position),
      vel(0.0f, 0.0f),
      force(0.0f, 0.0f),
      element(elementType)

{
}
void Atom::resetForce()
{
    force = sf::Vector2f(0.0f, 0.0f);
}
void Atom::applyForce(sf::Vector2f f)
{
    force += f;
}
void Atom::update(float dt, const Bounds &simulationBounds)
{
    vel += (force / mass()) * dt;
    pos += vel * dt;

    if (pos.x - radius() < simulationBounds.left)
    {
        pos.x = simulationBounds.left + radius();
        vel.x = std::abs(vel.x);
    }

    if (pos.x + radius() > simulationBounds.right)
    {
        pos.x = simulationBounds.right - radius();
        vel.x = -std::abs(vel.x);
    }

    if (pos.y - radius() < simulationBounds.top)
    {
        pos.y = simulationBounds.top + radius();
        vel.y = std::abs(vel.y);
    }

    if (pos.y + radius() > simulationBounds.bottom)
    {
        pos.y = simulationBounds.bottom - radius();
        vel.y = -std::abs(vel.y);
    }
}
float Atom::radius() const
{
    return element ? element->radius : 0.0f;
}
float Atom::mass() const
{
    return element ? element->mass : 0.0f;
}
sf::Color Atom::color() const
{
    return element ? element->color : sf::Color::White;
}
int Atom::valence() const
{
    return element ? element->valence : 0;
}
bool Atom::canBond() const
{
    return element && bondCount < element->valence;
}