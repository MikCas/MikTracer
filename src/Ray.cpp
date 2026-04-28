#include "Ray.h"

Ray::Ray() : m_origin(Vec3(0, 0, 0)), m_direction(Vec3(0, 0, 0)) {}

Ray::Ray(const Vec3& origin, const Vec3& direction) : m_origin(origin), m_direction(direction) {}

const Vec3& Ray::origin() const {
    return m_origin;
}

const Vec3& Ray::direction() const {
    return m_direction;
}

Vec3 Ray::at(double t) const {
    return m_origin + t * m_direction;
}



