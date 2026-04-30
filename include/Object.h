#pragma once

#include "Utility.h"
#include "Vec3.h"
#include "Ray.h"
#include "Interval.h"
#include "Hit.h"

#include <vector>
#include <memory>


class Object {
public:
    virtual ~Object() = default;

    virtual bool hit(const Ray& r, Interval hitInterval, Hit& hitRecord) const = 0;
};

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

