#include "Scene.h"

Scene::Scene(const Camera& camera)
    : camera(camera) {}
    
void Scene::addSphere(const Vec3& center, double radius, std::shared_ptr<Material> mat) {
    world.add(std::make_shared<Sphere>(center, radius, mat));
}
