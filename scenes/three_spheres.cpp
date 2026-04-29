#include "Camera.h"
#include "ImageBuffer.h"
#include "Scene.h"
#include "Vec3.h"
#include "Material.h"
#include "Renderer.h"
#include "SceneRunner.h"

#include <memory>   

// Classic "Ray Tracing in One Weekend" hero shot:
//   - large diffuse ground
//   - matte sphere on the left
//   - glass sphere in the centre
//   - polished metal sphere on the right

void buildScene(Scene& scene) {
    // Ground
    scene.world.add(std::make_shared<Sphere>(
        Vec3(0.0, -100.5, -1.0), 100.0,
        std::make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0))));

    // Left — matte red
    scene.world.add(std::make_shared<Sphere>(
        Vec3(-1.0, 0.0, -1.0), 0.5,
        std::make_shared<Lambertian>(Vec3(0.7, 0.3, 0.3))));

    // Centre — glass
    scene.world.add(std::make_shared<Sphere>(
        Vec3(0.0, 0.0, -1.0), 0.5,
        std::make_shared<Dielectric>(1.5)));

    // Right — polished metal
    scene.world.add(std::make_shared<Sphere>(
        Vec3(1.0, 0.0, -1.0), 0.5,
        std::make_shared<Metal>(Vec3(0.8, 0.8, 0.8), 0.0)));
}

Camera buildCamera() {
    CameraSettings cs;
    cs.lookFrom        = Vec3(0.0, 0.5, 2.5);
    cs.lookAt          = Vec3(0.0, 0.0, -1.0);
    cs.aspectRatio     = 16.0 / 9.0;
    cs.imageWidth      = 480;
    cs.verticalFOV     = 30.0;
    cs.focusDistance   = (cs.lookFrom - cs.lookAt).length();
    cs.defocusAngle    = 0.0;

    return Camera(cs);
}

int main() {
    return runScene("three_spheres", buildCamera(),
                    {.samplesPerPixel = 50, .maxDepth = 50},
                    [] {
        Scene scene;
        buildScene(scene);
        return scene;
    });
}