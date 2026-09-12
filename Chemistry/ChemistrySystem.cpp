#include "Chemistry/ChemistrySystem.hpp"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <SFML/System/Vector2.hpp>

constexpr float chemicalScale = 0.55f;
constexpr float bondSpringStiffness = 50.0f;

float ChemistrySystem::calculateFormationDistance(const Atom &first, const Atom &second, int order) const
{
    if (!first.element || !second.element)
        return 0.0f;

    float scaleByOrder = (order == 1)? 1.0f : (order == 2) ? 0.88f : 0.78f;
    float bondDistance = (first.element->covalentRadius + second.element->covalentRadius) * chemicalScale * scaleByOrder;
    return bondDistance * 1.2;
}
float ChemistrySystem::calculateBreakDistance(const Atom &first, const Atom &second, int order) const
{
    if (!first.element || !second.element)
        return 0.0f;

    float scaleByOrder = (order == 1)? 1.0f : (order == 2) ? 0.88f : 0.78f;
    float bondDistance = (first.element->covalentRadius + second.element->covalentRadius) * chemicalScale * scaleByOrder;
    float breakDistance = bondDistance * 1.5f;
    return breakDistance;
}

bool ChemistrySystem::canCreateBond(const Atom &first, const Atom &second) const
{
    if (!first.element || !second.element || first.id == second.id)
        return false;

    if (!first.canBond() || !second.canBond())
        return false;
    if (first.hasBondWith(second.id))
        return false;

    sf::Vector2f delta = second.pos - first.pos;
    float distanceSquared = delta.x * delta.x + delta.y * delta.y;
    float formationDistance = calculateFormationDistance(first, second);

    return distanceSquared < formationDistance * formationDistance;
}
bool ChemistrySystem::canUpgradeBond(const Atom &first, const Atom &second) const
{
    if (!first.element || !second.element || first.id == second.id)
        return false;

    const Bond* bond = first.getBondWith(second.id);
    if (!bond)
        return false;
    if (!first.canBond() || !second.canBond())
        return false;

    if (bond->order >= 3)
        return false;
    
    sf::Vector2f delta = second.pos - first.pos;
    float distanceSquared = delta.x * delta.x + delta.y * delta.y;
    float targetDistance = calculateFormationDistance(first, second, bond->order + 1);

    return distanceSquared < targetDistance * targetDistance;

}   
bool ChemistrySystem::createBond(Atom &first, Atom &second, int order = 1)
{
    if (!canCreateBond(first, second))
        return false;
    bool firstAdded = first.addBond(second.id, order);
    bool secondAdded = second.addBond(first.id, order);

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
bool ChemistrySystem::upgradeBond(Atom &first, Atom &second)
{
    if (!canUpgradeBond(first, second))
        return false;

    bool firstUpgraded = first.upgradeBondWith(second.id);
    bool secondUpgraded = second.upgradeBondWith(first.id);

    if (!firstUpgraded || !secondUpgraded)
    {
        if (firstUpgraded)
            first.downgradeBondWith(second.id); // Revert the upgrade
        if (secondUpgraded)
            second.downgradeBondWith(first.id); // Revert the upgrade
        return false;
    }

    std::cout << "Bond upgraded: atom " << first.id
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
            if (canUpgradeBond(atoms[i], atoms[j]))
            {
                upgradeBond(atoms[i], atoms[j]);
            }
            else if (canCreateBond(atoms[i], atoms[j]))
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
                const Bond &bond = currentAtom.bonds[j - 1];

                float breakDistance = calculateBreakDistance(currentAtom, *neighborAtom, bond.order);
                
                sf::Vector2f delta = neighborAtom->pos - currentAtom.pos;
                float distanceSquared = delta.x * delta.x + delta.y * delta.y;

                if (distanceSquared > breakDistance * breakDistance)
                {
                    currentAtom.downgradeBondWith(neighborId);
                    neighborAtom->downgradeBondWith(currentAtom.id);
                    if (currentAtom.hasBondWith(neighborId))
                    {
                        std::cout << "Bond downgraded: atom " << currentAtom.id
                                  << " (" << currentAtom.element->symbol << ") <-> atom "
                                  << neighborAtom->id << " (" << neighborAtom->element->symbol << ")\n";
                    }
                    else
                    {
                        std::cout << "Bond removed: atom " << currentAtom.id
                                  << " (" << currentAtom.element->symbol << ") <-> atom "
                                  << neighborAtom->id << " (" << neighborAtom->element->symbol << ")\n";
                    }    
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
            
            Atom *neighbor = simulation.findAtomById(bond.neighborId);
            if (!neighbor)
                continue;
            
            sf::Vector2f delta = neighbor->pos - atom.pos;
            float distanceSquared = delta.x * delta.x + delta.y * delta.y;
            if (distanceSquared < 0.0001f)
                continue;
            float distance = std::sqrt(distanceSquared);
            sf::Vector2f direction = delta / distance;
                      
            float orderLengthScale = (bond.order == 1) ? 1.0f : (bond.order == 2) ? 0.88f : 0.78f;
            float equilibriumDistance =
                (atom.element->covalentRadius + neighbor->element->covalentRadius) * chemicalScale * orderLengthScale;
            
            float effectiveStiffness = bondSpringStiffness * (1.0f + 0.8f * (bond.order - 1));
            float springForce = effectiveStiffness * (distance - equilibriumDistance);
            
            sf::Vector2f relativeVel = neighbor->vel - atom.vel;
            float velAlongBond = relativeVel.x * direction.x + relativeVel.y * direction.y;
            constexpr float dampingCoeff = 6.0f;
            float dampingForce = dampingCoeff * velAlongBond;
            

            sf::Vector2f force = direction * (springForce + dampingForce);
            atom.applyForce(force);
            neighbor->applyForce(-force);
        }
    }
}