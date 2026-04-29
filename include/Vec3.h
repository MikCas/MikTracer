#pragma once

#include "Utility.h"

#include <iostream>
#include <cmath>

struct Vec3 {

    double e[3];

    Vec3();
    Vec3(double e0, double e1, double e2);

    double x() const;
    double y() const;
    double z() const;

    Vec3 operator-() const;
    double operator[](int i) const;
    double& operator[](int i);
    Vec3& operator+=(const Vec3 &v);
    Vec3& operator*=(const double t);
    Vec3& operator/=(const double t);
    double lengthSquared() const;
    double length() const;

    static Vec3 random();
    static Vec3 random(double min, double max);

    bool nearZero() const;
};

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(double t, const Vec3& v) {
    return Vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vec3 operator*(const Vec3& v, double t) {
    return t * v;
}

inline Vec3 operator/(Vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const Vec3& u, const Vec3& v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 normalise(Vec3 v) {
    return v / v.length();
}

// Linear Interpolation
inline Vec3 lerp(double t, const Vec3& a, const Vec3& b) {
    return (1.0 - t) * a + t * b;
}

inline Vec3 randomUnitVector() {
    while(true) {
        Vec3 p = Vec3::random(-1, 1);
        double lengthSquared = p.lengthSquared();
        if(lengthSquared <= 1){
            return p/sqrt(lengthSquared);
        }
    }
}

inline Vec3 randomVectorOnUnitDisk() {
    while (true) {
        Vec3 p = Vec3(randomDouble(-1,1), randomDouble(-1,1), 0);
        if (p.lengthSquared() < 1)
            return p;
    }
}

inline Vec3 randomVectorOnHemisphere(Vec3 normal) {
    Vec3 vectorOnUnitSphere = randomUnitVector();
    if(dot(vectorOnUnitSphere, normal) > 0.0) { // In the same hemisphere as the normal
        return vectorOnUnitSphere;
    } else {
        return -vectorOnUnitSphere;
    }
}

inline Vec3 reflect(const Vec3& v, const Vec3& normal) {
    return v - 2*dot(v,normal)*normal;
}

inline Vec3 refract(const Vec3& v, const Vec3& normal, double refractiveIndexRatio){
    double cosTheta = std::fmin(dot(-v, normal), 1.0);
    Vec3 perpendicular = refractiveIndexRatio * (v + cosTheta * normal);
    Vec3 parallel = -std::sqrt(std::fabs(1.0 - perpendicular.lengthSquared())) * normal;
    return perpendicular + parallel;
}

inline bool operator==(const Vec3& u, const Vec3& v) {
    return u.e[0] == v.e[0] && u.e[1] == v.e[1] && u.e[2] == v.e[2];
}

inline bool operator!=(const Vec3& u, const Vec3& v) {
    return !(u == v);
}