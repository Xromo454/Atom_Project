#include "Simulation.hpp"

AtomStableId Simulation::addAtom(
    sf::Vector2f position,
    const ElementType *elementType)
{
    AtomStableId atomId = nextAtomId++;
    atoms.emplace_back(position, elementType, atomId);
    atomPositions[atomId] = atoms.size() - 1;

    return atomId;
}

bool Simulation::removeAtom(AtomStableId atomId)
{
    auto positionIt = atomPositions.find(atomId);

    if (positionIt == atomPositions.end())
        return false;

    std::size_t removedIndex = positionIt->second;

    for (Atom &atom : atoms)
    {
        if (atom.id != atomId)
            atom.removeBondWith(atomId);
    }

    std::size_t lastIndex = atoms.size() - 1;

    if (removedIndex != lastIndex)
    {
        atoms[removedIndex] = std::move(atoms[lastIndex]);
        atomPositions[atoms[removedIndex].id] = removedIndex;
    }

    atomPositions.erase(atomId);
    atoms.pop_back();

    return true;
}

Atom *Simulation::findAtomById(AtomStableId atomId)
{
    auto positionIt = atomPositions.find(atomId);

    if (positionIt == atomPositions.end())
        return nullptr;

    return &atoms[positionIt->second];
}

const Atom *Simulation::findAtomById(AtomStableId atomId) const
{
    auto positionIt = atomPositions.find(atomId);

    if (positionIt == atomPositions.end())
        return nullptr;

    return &atoms[positionIt->second];
}

std::vector<Atom> &Simulation::getAtoms()
{
    return atoms;
}

const std::vector<Atom> &Simulation::getAtoms() const
{
    return atoms;
}

std::size_t Simulation::size() const
{
    return atoms.size();
}

void Simulation::clear()
{
    atoms.clear();
    atomPositions.clear();
}
