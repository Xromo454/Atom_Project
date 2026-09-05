#include "Chemistry/ChemistrySystem.hpp"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <SFML/System/Vector2.hpp>

constexpr float chemicalScale = 0.55f; // changeable
float ChemistrySystem::calculateFormationDistance(const Atom &first, const Atom &second) const
{
    if (!first.element || !second.element)
        return 0.0f;

    float bondDistance = (first.element->covalentRadius + second.element->covalentRadius) * chemicalScale;
    float formationDistance = bondDistance * 1.2f; // number 1.2 is changeable
    return formationDistance;
}
float ChemistrySystem::calculateBreakDistance(const Atom &first, const Atom &second) const
{
    if (!first.element || !second.element)
        return 0.0f;

    float bondDistance = (first.element->covalentRadius + second.element->covalentRadius) * chemicalScale;
    float breakDistance = bondDistance * 1.5f; // number 1.5 is changeable
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

void ChemistrySystem::detectBonds(std::vector<Atom> &atoms)
{
    for (std::size_t i = 0; i < atoms.size(); ++i)
    {
        for (std::size_t j = i + 1; j < atoms.size(); ++j)
        {
            float formationDistance = calculateFormationDistance(atoms[i], atoms[j]);
            sf::Vector2f delta = atoms[j].pos - atoms[i].pos;
            float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

            if (distance <= formationDistance * 2.0f)
            {
                std::cout << "Bond check: atom " << atoms[i].id
                          << "-" << atoms[j].id
                          << ", distance = " << distance
                          << ", formation distance = " << formationDistance
                          << '\n';
            }

            if (canCreateBond(atoms[i], atoms[j]))
            {
                createBond(atoms[i], atoms[j]);
            }
        }
    }
}

void ChemistrySystem::removeBrokenBonds(std::vector<Atom> &atoms)
{
    for (std::size_t i = 0; i < atoms.size(); ++i)
    {
        Atom &currentAtom = atoms[i];
        for (std::size_t j = 0; j < currentAtom.bonds.size(); ++j)
        {
            AtomStableId neighborId = currentAtom.bonds[j].neighborId;
            auto neighborIt = std::find_if(atoms.begin(), atoms.end(), [neighborId](const Atom &atom)
                                           { return atom.id == neighborId; });
            if (neighborIt != atoms.end())
            {
                Atom &neighborAtom = *neighborIt;
                float breakDistance = calculateBreakDistance(currentAtom, neighborAtom);
                sf::Vector2f delta = neighborAtom.pos - currentAtom.pos;
                float distanceSquared = delta.x * delta.x + delta.y * delta.y;

                if (distanceSquared > breakDistance * breakDistance)
                {
                    removeBond(currentAtom, neighborAtom);
                }
            }
        }
    }
}

void ChemistrySystem::updateBonds(std::vector<Atom> &atoms)
{
    removeBrokenBonds(atoms);
    detectBonds(atoms);
}