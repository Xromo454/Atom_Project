#pragma once

#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Entities/Atom.hpp"

class Simulation
{
private:
    std::vector<Atom> atoms;
    std::unordered_map<AtomStableId, std::size_t> atomPositions;
    AtomStableId nextAtomId = 0;

public:
    AtomStableId addAtom(sf::Vector2f position, const ElementType *elementType);
    bool removeAtom(AtomStableId atomId);

    Atom *findAtomById(AtomStableId atomId);
    const Atom *findAtomById(AtomStableId atomId) const;

    std::vector<Atom> &getAtoms();
    const std::vector<Atom> &getAtoms() const;

    std::size_t size() const;
    void clear();
};
