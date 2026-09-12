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
        {"H",  "Hydrogen", sf::Color(235, 235, 235),  1.0f,  5.5f,  8.0f,  50.0f, 22.0f, 1, 1},
        {"He", "Helium",   sf::Color(160, 230, 255),  4.0f, 10.0f,  0.0f,  10.0f, 26.0f, 0, 2},
        {"C",  "Carbon",   sf::Color(110, 110, 110), 12.0f, 12.0f, 19.0f, 400.0f, 34.0f, 4, 6},
        {"N",  "Nitrogen", sf::Color(100, 100, 255), 14.0f, 11.0f, 17.5f, 250.0f, 31.0f, 3, 7},
        {"O",  "Oxygen",   sf::Color(240,  70,  70), 16.0f, 10.0f, 16.5f, 300.0f, 30.5f, 2, 8}
};