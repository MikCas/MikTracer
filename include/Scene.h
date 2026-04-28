#pragma once

#include "Camera.h"
#include "Material.h"
#include "Object.h"
#include "Vec3.h"

#include <memory>

class Scene {
public:
    ObjectList world;
    Camera     camera;

    Scene(const Camera& cam);

    void addSphere(const Vec3& center, double radius, std::shared_ptr<Material> mat);

    std::shared_ptr<Lambertian> lambertian(const Vec3& albedo);
    std::shared_ptr<Metal> metal(const Vec3& albedo, double fuzz);
    std::shared_ptr<Dielectric> dielectric(double refractiveIndex);
};
