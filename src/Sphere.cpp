#include "../include/Sphere.h"

Sphere::Sphere() : m_center(Vec3(0, 0, 0)), m_radius(1) {}

Sphere::Sphere(const Vec3& center, double radius) : m_center(center), m_radius(std::fmax(0, radius)) {}

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

    return true;
}