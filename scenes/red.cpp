#include "Camera.h"
#include "Scene.h"
#include "Vec3.h"
#include "Material.h"
#include "SceneRunner.h"

#include <memory>   

void buildScene(Scene& scene) {
    auto ground = std::make_shared<Lambertian>(Vec3(0.1, 0.1, 0.1));
    auto red_metal = std::make_shared<Metal>(Vec3(0.7, 0.1, 0.1), 0.2);

    scene.world.add(std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, ground));

    // Create "Monoliths" using large spheres stretched by distance
    for (int i = 0; i < 8; ++i) {
        double z_pos = -i * 10.0 - 5.0;
        // Massive scale: radius of 10-20 units
        scene.world.add(std::make_shared<Sphere>(Vec3(-15, 10, z_pos), 15, red_metal));
        scene.world.add(std::make_shared<Sphere>(Vec3(15, 10, z_pos), 15, red_metal));
    }

    // Reference object: A tiny white sphere near the camera to give a "human" scale
    scene.world.add(std::make_shared<Sphere>(Vec3(0, -0.4, 1), 0.1, std::make_shared<Lambertian>(Vec3(1,1,1))));
}

Camera buildCamera() {
    CameraSettings cs;
    cs.lookFrom = Vec3(0, 0, 5); // Low to the ground
    cs.lookAt   = Vec3(0, 2, -10); // Looking up and into the distance
    cs.verticalFOV = 60.0; // Wide angle to enhance the sense of vastness
    return Camera(cs);
}

int main() {
    return runScene("red", buildCamera(), 
                    {.samplesPerPixel = 10, .maxDepth = 10}, 
                    [] {
        Scene scene;
        buildScene(scene);
        return scene;
    });
}