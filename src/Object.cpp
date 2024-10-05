#include "../include/Object.h"

////////////////////////////////////// OBJECT LIST CLASS //////////////////////////////////////

ObjectList::ObjectList() {}

ObjectList::ObjectList(std::shared_ptr<Object> object) {
    add(object);
}

void ObjectList::clear() {
    m_objects.clear();
}

void ObjectList::add(std::shared_ptr<Object> object) {
    m_objects.push_back(object);
}

bool ObjectList::hit(const Ray& r, Interval hitInterval, Hit& hitRecord) const {
    Hit tempHit;
    bool hitAnything = false;
    double closestSoFar = hitInterval.max;

    for (const auto& object : m_objects) {
        if (object->hit(r, Interval(hitInterval.min, closestSoFar), tempHit)) {
            hitAnything = true;
            closestSoFar = tempHit.t;
            hitRecord = tempHit;
        }
    }

    return hitAnything;
}

////////////////////////////////////// SPHERE CLASS //////////////////////////////////////

Sphere::Sphere() : m_center(Vec3(0, 0, 0)), m_radius(1) {}

Sphere::Sphere(const Vec3& center, double radius, std::shared_ptr<Material> material) 
: m_center(center), m_radius(std::fmax(0, radius)), m_material(material) 
{}

bool Sphere::hit(const Ray& r, Interval hitInterval, Hit& hitRecord) const {
    Vec3 oc = m_center - r.origin(); 
    double a = r.direction().lengthSquared();
    double h = dot(r.direction(), oc);
    double c = oc.lengthSquared() - m_radius*m_radius;

    double discriminant = h*h - a*c;
    if (discriminant < 0) {
        return false;
    }

    double sqrtDiscriminant = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range
    double root = (h - sqrtDiscriminant) / a;
    if(!hitInterval.surrounds(root)) {
        root = (h + sqrtDiscriminant) / a;
        if(!hitInterval.surrounds(root)) {
            return false;
        }
    }
    
    hitRecord.t = root;
    hitRecord.position = r.at(hitRecord.t);
    Vec3 outNormal = (hitRecord.position - m_center) / m_radius;
    hitRecord.setNormal(r, outNormal);
    hitRecord.material = m_material;

    return true;
}