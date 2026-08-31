#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "../Atom.hpp"

class PhysicsSystem
{
    public:
        sf::Vector2f computePairForce(const Atom& a, const Atom& b);
        void applyInteractions(std::vector<Atom>& atoms);
};