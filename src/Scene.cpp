#include "Scene.h"

Scene::Scene(const Camera& camera)
    : camera(camera) {}
    
void Scene::addSphere(const Vec3& center, double radius, std::shared_ptr<Material> mat) {
    world.add(std::make_shared<Sphere>(center, radius, mat));
}

std::shared_ptr<Lambertian> Scene::lambertian(const Vec3& albedo) {
    return std::make_shared<Lambertian>(albedo);
}

std::shared_ptr<Metal> Scene::metal(const Vec3& albedo, double fuzz) {
    return std::make_shared<Metal>(albedo, fuzz);
}

std::shared_ptr<Dielectric> Scene::dielectric(double refractiveIndex) {
    return std::make_shared<Dielectric>(refractiveIndex);
}
