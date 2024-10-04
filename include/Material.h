#pragma once

#include "Object.h"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& inputRay, const Hit& rec, Vec3& attenuation, Ray& scatteredRay) const {
        return false;
    }
};


