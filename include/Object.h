#pragma once

#include "Ray.h"

struct Hit {
    Vec3 position;
    Vec3 normal;
    double t;
    bool frontFace;

    inline void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Object {
public:
    virtual ~Object() = default;

    virtual bool hit(const Ray& r, double tMin, double tMax, Hit& hitRecord) const = 0;
};


