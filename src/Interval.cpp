#include "../include/Interval.h"

Interval::Interval() : min(+infinity), max(-infinity) {}

Interval::Interval(double min, double max) : min(min), max(max) {}

double Interval::size() const {
    return max - min;
}

bool Interval::contains(double x) const {
    return min <= x && x <= max;
}

bool Interval::surrounds(double x) const {
    return min < x && x < max;
}

double Interval::clamp(double x) const {
    return std::clamp(x, min, max);
}

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);



