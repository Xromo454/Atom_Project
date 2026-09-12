#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include "Element.hpp"
#include "Core/Bounds.hpp"

using AtomStableId = std::size_t;
struct Bond
{
    AtomStableId neighborId;
    int order = 1;
};

struct Atom
{
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f force;

    const ElementType *element;

    bool isInMolecule = false;
    int moleculeId = -1;

    AtomStableId id;
    std::vector<Bond> bonds;
    size_t maxBonds;

    Atom(sf::Vector2f position, const ElementType *elementType, AtomStableId identifier);
    //Physics and movement
    void resetForce();
    void applyForce(sf::Vector2f f);
    void update(float dt, const Bounds &simulationBounds);
    //Getters
    float radius() const;
    float mass() const;
    sf::Color color() const;
    int valence() const;
    //Chemistry and bonding
    int usedValence() const;
    bool canBond(int order = 1) const;
    const Bond* getBondWith(const AtomStableId targetId) const;
    Bond* getBondWith(const AtomStableId targetId);
    bool upgradeBondWith(const AtomStableId targetId);
    bool downgradeBondWith(const AtomStableId targetId);
    bool hasBondWith(const AtomStableId targetId) const;
    bool addBond(const AtomStableId targetId, int order = 1);
    bool removeBondWith(const AtomStableId targetId);
};