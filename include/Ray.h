#pragma once

#include "Vec3.h"

class Ray {
private:
    Vec3 m_origin;
    Vec3 m_direction;

public:

    // Constructors
    Ray();
    Ray(const Vec3& origin, const Vec3& direction);

    // Getters
    const Vec3& origin() const;
    const Vec3& direction() const;

    // Operations
    Vec3 at(double t) const;
};
