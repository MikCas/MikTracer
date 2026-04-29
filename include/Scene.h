#pragma once

#include "Camera.h"
#include "Material.h"
#include "Object.h"
#include "Vec3.h"

#include <memory>


struct Scene {
    ObjectList world;
    Camera     camera;

    Scene(const Camera& camera);

    void addSphere(const Vec3& center, double radius, std::shared_ptr<Material> mat);
};
