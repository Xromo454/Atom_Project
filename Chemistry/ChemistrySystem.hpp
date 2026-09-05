#pragma once
#include "Core/Entities/Atom.hpp"

class ChemistrySystem
{
public:
    float calculateFormationDistance(const Atom &first, const Atom &second) const;
    float calculateBreakDistance(const Atom &first, const Atom &second) const;
    bool canCreateBond(const Atom &first, const Atom &second) const;
    bool createBond(Atom &first, Atom &second);
    bool removeBond(Atom &first, Atom &second);
    void detectBonds(std::vector<Atom> &atoms);
    void removeBrokenBonds(std::vector<Atom> &atoms);
    void updateBonds(std::vector<Atom> &atoms);
};
