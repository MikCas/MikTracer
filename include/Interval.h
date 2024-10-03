#pragma once

#include "Utility.h"

struct Interval {
    double min;
    double max;

    Interval();
    Interval(double tMin, double tMax);

    double size() const;
    bool contains(double x) const;
    bool surrounds(double x) const;
    double clamp(double x) const;

    static const Interval empty, universe;
};
