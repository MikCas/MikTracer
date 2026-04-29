#include "Camera.h"
#include "Scene.h"
#include "Vec3.h"
#include "Material.h"
#include "SceneRunner.h"

#include <memory>   

void buildScene(Scene& scene) {
    scene.world.add(std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, std::make_shared<Lambertian>(Vec3(0.05, 0.05, 0.1))));

    for (int i = 0; i < 200; i++) {
        Vec3 center(randomDouble(-5, 5), randomDouble(0, 5), randomDouble(-10, 0));
        double choose_mat = randomDouble();

        if (choose_mat < 0.7) {
            // Various shades of blue
            auto albedo = Vec3(randomDouble(0, 0.2), randomDouble(0, 0.5), randomDouble(0.6, 1.0));
            scene.world.add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Lambertian>(albedo)));
        } else if (choose_mat < 0.9) {
            // Frosted blue metal
            scene.world.add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Metal>(Vec3(0.7, 0.8, 1.0), 0.3)));
        } else {
            // Glass bubbles
            scene.world.add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Dielectric>(1.5)));
        }
    }
}

Camera buildCamera() {
    CameraSettings cs;
    cs.lookFrom = Vec3(0, 2, 6);
    cs.lookAt   = Vec3(0, 1, 0);
    cs.verticalFOV = 45.0;
    cs.defocusAngle = 0.5; // Slight blur to create "bokeh" in the random particles
    cs.focusDistance = 6.0;
    return Camera(cs);
}

int main() {
    return runScene("portfolio_blue", buildCamera(), 
                    {.samplesPerPixel = 10, .maxDepth = 10}, 
                    [] {
        Scene scene;
        buildScene(scene);
        return scene;
    });
}