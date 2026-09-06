#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

struct ElementType
{
    std::string symbol;
    std::string name;
    sf::Color color;

    float mass;
    float radius;
    float covalentRadius;
    float epsilon;
    float sigma;

    int valence;
    int atomic_number;

    ElementType(std::string symbol, std::string name, sf::Color color, float mass, float radius, float covalentRadius, float epsilon, float sigma, int valence, int atomic_number)
        : symbol(std::move(symbol)),
          name(std::move(name)),
          color(std::move(color)),
          mass(mass),
          radius(radius),
          covalentRadius(covalentRadius),
          epsilon(epsilon),
          sigma(sigma),
          valence(valence),
          atomic_number(atomic_number)
    {
    }
};

inline const std::vector<ElementType> ELEMENTS =
    {
        // Symbol, name, color, mass, visual radius, covalent radius, LJ epsilon, LJ sigma, valence, atomic number.
        {"H", "Hydrogen", sf::Color(235, 235, 235), 1.0f, 12.0f, 31, 150.0f, 24.0f, 1, 1},
        {"He", "Helium", sf::Color(160, 230, 255), 4.0f, 10.5f, 0, 10.0f, 28.0f, 0, 2},
        {"C", "Carbon", sf::Color(90, 90, 90), 12.0f, 28.0f, 76, 400.0f, 34.0f, 4, 6},
        {"N", "Nitrogen", sf::Color(100, 100, 255), 14.0f, 26.5f, 71, 250.0f, 31.0f, 3, 7},
        {"O", "Oxygen", sf::Color(240, 70, 70), 16.0f, 23.5f, 66, 300.0f, 30.5f, 2, 8}

};