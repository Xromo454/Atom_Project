#pragma once
#include <SFML/System/Vector2.hpp>
struct Atom
{
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f force;
    float mass;
    float radius;

    Atom(sf::Vector2f position, float m = 1.0f, float r = 8.0f);

    void resetForce();

    void applyForce(sf::Vector2f f);

    void update(float dt);
};