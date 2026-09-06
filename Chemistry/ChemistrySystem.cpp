#include "Chemistry/ChemistrySystem.hpp"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <SFML/System/Vector2.hpp>

constexpr float chemicalScale = 0.55f;
constexpr float bondSpringStiffness = 50.0f;

float ChemistrySystem::calculateFormationDistance(const Atom &first, const Atom &second) const
{
    if (!first.element || !second.element)
        return 0.0f;

    float bondDistance = (first.element->covalentRadius + second.element->covalentRadius) * chemicalScale;
    float formationDistance = bondDistance * 1.2f;
    return formationDistance;
}
float ChemistrySystem::calculateBreakDistance(const Atom &first, const Atom &second) const
{
    if (!first.element || !second.element)
        return 0.0f;

    float bondDistance = (first.element->covalentRadius + second.element->covalentRadius) * chemicalScale;
    float breakDistance = bondDistance * 1.5f;
    return breakDistance;
}

bool ChemistrySystem::canCreateBond(const Atom &first, const Atom &second) const
{
    if (!first.element || !second.element)
        return false;

    if (first.id == second.id)
        return false;

    if (!first.canBond() || !second.canBond())
        return false;

    if (first.hasBondWith(second.id) || second.hasBondWith(first.id))
        return false;

    sf::Vector2f delta = second.pos - first.pos;
    float distanceSquared = delta.x * delta.x + delta.y * delta.y;
    float formationDistance = calculateFormationDistance(first, second);
    return distanceSquared < formationDistance * formationDistance;
}

bool ChemistrySystem::createBond(Atom &first, Atom &second)
{
    if (!canCreateBond(first, second))
        return false;
    bool firstAdded = first.addBond(second.id);
    bool secondAdded = second.addBond(first.id);

    if (!firstAdded || !secondAdded)
    {
        if (firstAdded)
            first.removeBondWith(second.id);
        if (secondAdded)
            second.removeBondWith(first.id);
        return false;
    }

    std::cout << "Bond created: atom " << first.id
              << " (" << first.element->symbol << ") <-> atom "
              << second.id << " (" << second.element->symbol << ")\n";

    return true;
}

bool ChemistrySystem::removeBond(Atom &first, Atom &second)
{
    if (!first.hasBondWith(second.id) || !second.hasBondWith(first.id))
        return false;

    first.removeBondWith(second.id);
    second.removeBondWith(first.id);
    return true;
}

void ChemistrySystem::detectBonds(Simulation &simulation)
{
    std::vector<Atom> &atoms = simulation.getAtoms();

    for (std::size_t i = 0; i < atoms.size(); ++i)
    {
        for (std::size_t j = i + 1; j < atoms.size(); ++j)
        {
            if (canCreateBond(atoms[i], atoms[j]))
            {
                createBond(atoms[i], atoms[j]);
            }
        }
    }
}

void ChemistrySystem::removeBrokenBonds(Simulation &simulation)
{
    std::vector<Atom> &atoms = simulation.getAtoms();

    for (std::size_t i = atoms.size(); i > 0; --i)
    {
        Atom &currentAtom = atoms[i - 1];
        for (std::size_t j = currentAtom.bonds.size(); j > 0; --j)
        {
            AtomStableId neighborId = currentAtom.bonds[j - 1].neighborId;
            Atom *neighborAtom = simulation.findAtomById(neighborId);

            if (neighborAtom != nullptr)
            {
                float breakDistance = calculateBreakDistance(currentAtom, *neighborAtom);
                sf::Vector2f delta = neighborAtom->pos - currentAtom.pos;
                float distanceSquared = delta.x * delta.x + delta.y * delta.y;

                if (distanceSquared > breakDistance * breakDistance)
                {
                    std::cout << "Bond broken: atom "
                              << currentAtom.id << " ("
                              << currentAtom.element->symbol << ") <-> atom "
                              << neighborAtom->id << " ("
                              << neighborAtom->element->symbol << ")\n";
                    removeBond(currentAtom, *neighborAtom);
                }
            }
        }
    }
}

void ChemistrySystem::updateBonds(Simulation &simulation)
{
    removeBrokenBonds(simulation);
    detectBonds(simulation);
}

void ChemistrySystem::applyBondForces(Simulation &simulation)
{
    std::vector<Atom> &atoms = simulation.getAtoms();

    for (Atom &atom : atoms)
    {
        for (const Bond &bond : atom.bonds)
        {
            if (atom.id >= bond.neighborId)
                continue;

            AtomStableId neighborId = bond.neighborId;

            Atom *neighbor = simulation.findAtomById(neighborId);

            if (neighbor == nullptr)
                continue;

            sf::Vector2f delta = neighbor->pos - atom.pos;
            float distanceSquared = delta.x * delta.x + delta.y * delta.y;

            if (distanceSquared < 0.0001f)
                continue;

            float distance = std::sqrt(distanceSquared);
            sf::Vector2f direction = delta / distance;
            float equilibriumDistance =
                (atom.element->covalentRadius + neighbor->element->covalentRadius) * chemicalScale;

            float forceMagnitude =
                bondSpringStiffness * (distance - equilibriumDistance);
            sf::Vector2f force = direction * forceMagnitude;

            atom.applyForce(force);
            neighbor->applyForce(-force);
        }
    }
}