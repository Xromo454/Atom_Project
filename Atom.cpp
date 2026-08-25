#include "Atom.hpp"

Atom::Atom(sf::Vector2f position, float m, float r)
    : pos(position), vel(0.0f, 0.0f), force(0.0f, 0.0f), mass(m), radius(r) {}

void Atom::resetForce()
{
    force = sf::Vector2f(0.0f, 0.0f);
}

void Atom::applyForce(sf::Vector2f f)
{
    force += f;
}

void Atom::update(float dt)
{
    sf::Vector2f accel = force / mass;
    vel += accel * dt;
    pos += vel * dt;
    resetForce();
};
