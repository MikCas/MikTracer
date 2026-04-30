# pragma once 

#include "Vec3.h"
#include "Ray.h"

#include <memory>

class Material; // forward decleration

struct Hit {
    Vec3 position;
    Vec3 normal;
    std::shared_ptr<Material> material;
    double t;
    bool frontFace;

    // outNormal is the normal pointing out of the object, assumed to have unit length
    inline void setNormal(const Ray& r, const Vec3& outNormal) {
        frontFace = dot(r.direction(), outNormal) < 0;
        normal = frontFace ? outNormal : -outNormal;
    }
};
