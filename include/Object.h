#pragma once

#include "Utility.h"

struct Hit {
    Vec3 position;
    Vec3 normal;
    double t;
    bool frontFace;

    // NOTE: outNormal is the normal pointing out of the object, and is assumed to have unit length
    inline void setNormal(const Ray& r, const Vec3& outNormal) {
        frontFace = dot(r.direction(), outNormal) < 0;
        normal = frontFace ? outNormal : -outNormal;
    }
};

class Object {
public:
    virtual ~Object() = default;

    virtual bool hit(const Ray& r, Interval hitInterval, Hit& hitRecord) const = 0;
};


