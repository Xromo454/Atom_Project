#pragma once

#include "Core/Simulation.hpp"

class ChemistrySystem
{
public:
    float calculateFormationDistance(const Atom &first, const Atom &second, int order = 1) const;
    float calculateBreakDistance(const Atom &first, const Atom &second, int order = 1) const;

    bool canCreateBond(const Atom &first, const Atom &second) const;
    bool canUpgradeBond(const Atom &first, const Atom &second) const;

    bool createBond(Atom &first, Atom &second, int order);
    bool upgradeBond(Atom &first, Atom &second);
    bool removeBond(Atom &first, Atom &second);

    void detectBonds(Simulation &simulation);
    void removeBrokenBonds(Simulation &simulation);
    void updateBonds(Simulation &simulation);
    void applyBondForces(Simulation &simulation);
};
