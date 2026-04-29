#include "Camera.h"
#include "Scene.h"
#include "Vec3.h"
#include "Material.h"
#include "SceneRunner.h"

#include <memory>   

void buildScene(Scene& scene) {
    // Large "Mirror" Walls to create the box illusion
    auto mirror = std::make_shared<Metal>(Vec3(1.0, 1.0, 1.0), 0.0);
    auto green_matte = std::make_shared<Lambertian>(Vec3(0.12, 0.45, 0.15));

    // Floor, Back Wall, and Left Wall
    scene.world.add(std::make_shared<Sphere>(Vec3(0, -1000.5, 0), 1000, green_matte)); // Floor
    scene.world.add(std::make_shared<Sphere>(Vec3(0, 0, -1005), 1000, mirror));       // Back wall
    scene.world.add(std::make_shared<Sphere>(Vec3(-1005, 0, 0), 1000, mirror));      // Left wall

    // The "Hero" - A glass sphere refracting the green floor and mirrored walls
    scene.world.add(std::make_shared<Sphere>(Vec3(0, 0, -1), 0.5, std::make_shared<Dielectric>(1.5)));
    
    // A floating gold sphere to add a splash of contrast in the reflection
    scene.world.add(std::make_shared<Sphere>(Vec3(0.8, -0.2, -1.5), 0.3, std::make_shared<Metal>(Vec3(0.8, 0.6, 0.2), 0.1)));
}

Camera buildCamera() {
    CameraSettings cs;
    cs.lookFrom = Vec3(3, 2, 4);
    cs.lookAt   = Vec3(0, 0, -1);
    cs.verticalFOV = 40.0;
    return Camera(cs);
}

int main() {
    return runScene("green", buildCamera(), 
                    {.samplesPerPixel = 100, .maxDepth = 100}, 
                    [] {
        Scene scene;
        buildScene(scene);
        return scene;
    });
}