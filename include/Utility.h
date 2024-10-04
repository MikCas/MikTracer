#pragma once

// Common Headers

#include <iostream>
#include <cmath>
#include <limits>
#include <memory>
#include <fstream>
#include <random>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

// Random real-number generator
inline double randomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double randomDouble(double min, double max) {
    return min + (max - min) * randomDouble();
}
