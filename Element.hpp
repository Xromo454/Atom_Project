#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

struct ElementType
{
    std::string name;
    sf::Color color;
    float radius;
    float epsilon;
    float sigma;
};

// Element types with their properties
const std::vector<ElementType> ELEMENTS = {
    {"Hydrogen (H)", sf::Color(230, 230, 230), 12.0f, 30.0f, 2.0f},
    {"Helium (He)", sf::Color(150, 220, 255), 10.0f, 15.0f, 1.5f},
    {"Carbon (C)", sf::Color(80, 80, 80), 18.0f, 60.0f, 2.4f},
    {"Oxygen (O)", sf::Color(235, 60, 60), 15.0f, 50.0f, 2.0f}
};