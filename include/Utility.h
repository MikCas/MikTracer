#pragma once

// Common Headers

#include <iostream>
#include <cmath>
#include <limits>
#include <memory>
#include <fstream>

#include "Ray.h"
#include "Interval.h"
#include "Vec3.h"

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

// Linear Interpolation
inline Vec3 lerp(double t, const Vec3& a, const Vec3& b) {
    return (1.0 - t) * a + t * b;
}

