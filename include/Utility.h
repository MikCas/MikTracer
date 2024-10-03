#pragma once

#include <iostream>
#include <cmath>
#include <limits>
#include <memory>

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

// Common Headers

#include "Ray.h"
#include "Interval.h"
#include "Vec3.h"
#include "Color.h"  

