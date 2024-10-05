#pragma once

#include "Utility.h"

struct Interval {
    double min;
    double max;

    // Static members
    static const Interval empty, universe;

    // Constructors
    Interval();
    Interval(double tMin, double tMax);

    // Interval operations
    double size() const;
    bool contains(double x) const;
    bool surrounds(double x) const;
    double clamp(double x) const;

};
