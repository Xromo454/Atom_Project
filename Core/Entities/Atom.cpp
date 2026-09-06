#include "Atom.hpp"
#include "Core/Bounds.hpp"
#include <algorithm>
#include <cmath>
Atom::Atom(sf::Vector2f position, const ElementType *elementType, AtomStableId identifier)
    : pos(position),
      vel(0.0f, 0.0f),
      force(0.0f, 0.0f),
      element(elementType),
      id(identifier),
      maxBonds(elementType ? elementType->valence : 0)

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
void Atom::update(float dt, const Bounds &bounds)
{
    sf::Vector2f acceleration = force / mass();

    vel += acceleration * dt;

    float maxVel = 400.0f;
    float currentSpeed = std::hypot(vel.x, vel.y);
    if (currentSpeed > maxVel)
    {
        vel = (vel / currentSpeed) * maxVel;
    }

    vel *= 0.99f;

    pos += vel * dt;

    float r = radius();
    if (pos.x - r < bounds.left)
    {
        pos.x = bounds.left + r;
        vel.x = -vel.x * 0.7f;
    }
    else if (pos.x + r > bounds.right)
    {
        pos.x = bounds.right - r;
        vel.x = -vel.x * 0.7f;
    }

    if (pos.y - r < bounds.top)
    {
        pos.y = bounds.top + r;
        vel.y = -vel.y * 0.7f;
    }
    else if (pos.y + r > bounds.bottom)
    {
        pos.y = bounds.bottom - r;
        vel.y = -vel.y * 0.7f;
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
    return element && bonds.size() < maxBonds;
}
bool Atom::hasBondWith(const AtomStableId targetId) const
{
    return std::any_of(
        bonds.begin(),
        bonds.end(),
        [targetId](const Bond &bond)
        {
            return bond.neighborId == targetId;
        });
}
bool Atom::addBond(const AtomStableId targetId, int order)
{
    if (!canBond())
        return false;

    if (hasBondWith(targetId))
        return false;

    bonds.push_back({targetId, order});
    return true;
}
bool Atom::removeBondWith(const AtomStableId targetId)
{
    auto it = std::remove_if(
        bonds.begin(),
        bonds.end(),
        [targetId](const Bond &bond)
        {
            return bond.neighborId == targetId;
        });

    if (it != bonds.end())
    {
        bonds.erase(it, bonds.end());
        return true;
    }
    return false;
}
