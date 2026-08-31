#pragma once
#include <SFML/System/Vector2.hpp>

#include "Element.hpp" 
#include "Core/Bounds.hpp"
struct Atom
{
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f force; // Squared distance to another atom (used for force calculations)

    const ElementType* element;

    int bondCount = 0;
    bool isInMolecule = false;
    int moleculeId = -1;

    Atom(sf::Vector2f position, const ElementType* elementType);

    void resetForce();
    void applyForce(sf::Vector2f f);
    void update(float dt, const Bounds &simulationBounds);
    
    float radius() const;
    float mass() const;
    sf::Color color() const;
    int valence() const;
    bool canBond() const;

};