#pragma once

#include "Core/Simulation.hpp"

class ChemistrySystem
{
public:
    float calculateFormationDistance(const Atom &first, const Atom &second) const;
    float calculateBreakDistance(const Atom &first, const Atom &second) const;
    bool canCreateBond(const Atom &first, const Atom &second) const;
    bool createBond(Atom &first, Atom &second);
    bool removeBond(Atom &first, Atom &second);
    void detectBonds(Simulation &simulation);
    void removeBrokenBonds(Simulation &simulation);
    void updateBonds(Simulation &simulation);
    void applyBondForces(Simulation &simulation);
};
