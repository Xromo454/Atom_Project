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
    float epsilon;
    float sigma;

    int valence;
};

inline const std::vector<ElementType> ELEMENTS = 
{
    {"H", "Hydrogen", sf::Color(230, 230, 230), 1.0f, 12.0f, 30.0f, 2.0f, 1},
    {"He", "Helium", sf::Color(150, 220, 255), 4.0f, 10.0f, 15.0f, 1.5f, 2},
    {"C", "Carbon", sf::Color(80, 80, 80), 12.0f, 18.0f, 60.0f, 2.4f, 4},
    {"O", "Oxygen", sf::Color(235, 60, 60), 16.0f, 15.0f, 50.0f, 2.0f, 2}
};