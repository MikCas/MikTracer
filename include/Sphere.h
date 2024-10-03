#pragma once

#include "Object.h"

class Sphere : public Object {
private:
    Vec3 m_center;
    double m_radius;

public:
    Sphere();
    Sphere(const Vec3& center, double radius);

    bool hit(const Ray& r, double tMin, double tMax, Hit& hitRecord) const override;
};