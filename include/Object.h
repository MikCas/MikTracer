#pragma once

#include "Utility.h"
#include "Vec3.h"
#include "Ray.h"
#include "Interval.h"

// Forward declare material
class Material;

struct Hit {
    Vec3 position;
    Vec3 normal;
    std::shared_ptr<Material> material;
    double t;
    bool frontFace;

    // NOTE: outNormal is the normal pointing out of the object, and is assumed to have unit length
    inline void setNormal(const Ray& r, const Vec3& outNormal) {
        frontFace = dot(r.direction(), outNormal) < 0;
        normal = frontFace ? outNormal : -outNormal;
    }
};

////////////////////////////////////// ABSTRACT OBJECT CLASS //////////////////////////////////////
class Object {
public:
    virtual ~Object() = default;

    virtual bool hit(const Ray& r, Interval hitInterval, Hit& hitRecord) const = 0;
};

////////////////////////////////////// OBJECT LIST CLASS //////////////////////////////////////
class ObjectList : public Object {
private:
    std::vector<std::shared_ptr<Object>> m_objects;

public:
    ObjectList();
    ObjectList(std::shared_ptr<Object> object);

    void clear();
    void add(std::shared_ptr<Object> object);

    bool hit(const Ray& r, Interval hitInterval, Hit& hitRecord) const override;
};

////////////////////////////////////// SPHERE CLASS //////////////////////////////////////
class Sphere : public Object {
private:
    Vec3 m_center;
    double m_radius;
    std::shared_ptr<Material> m_material;

public:
    Sphere();
    Sphere(const Vec3& center, double radius, std::shared_ptr<Material> material);

    bool hit(const Ray& r, Interval hitInterval, Hit& hitRecord) const override;
};

