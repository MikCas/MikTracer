#include "Camera.h"
#include "Scene.h"
#include "Utility.h"
#include "Vec3.h"
#include "Material.h"
#include "SceneRunner.h"
#include <memory>  

// Colors
const Vec3 WHITE      = Vec3(1.0, 1.0, 1.0);
const Vec3 BLACK      = Vec3(0.0, 0.0, 0.0);

void buildScene(Scene& scene) {
    double radius = 0.5;
    double groundRadius = 100.0;
    int numCols = 6;
    int numRows = 10;
    double offset = numCols * radius;

    // Ground
    Vec3 groundPos(0.0, -groundRadius - radius, -1);
    scene.world.add(std::make_shared<Sphere>(
        groundPos, groundRadius, std::make_shared<Lambertian>(WHITE)
    ));

    // Grid of metal spheres
    for (int i = 0; i < numCols; ++i) {
        double xPos = 2 * radius * i - offset + radius;
        for (int j = 0; j < numRows; ++j) {
            double zPos = -2 * j * radius + 2 * radius;
            Vec3 pos(xPos, 0.0, zPos);

            double r = randomDouble(radius - 0.2, radius);
            Vec3 albedo = lerp(randomDouble(), WHITE, BLACK);
            scene.world.add(std::make_shared<Sphere>(
                pos, r, std::make_shared<Metal>(albedo, randomDouble())
            ));
        }
    }
}

Camera buildCamera() {
    CameraSettings cs;
    cs.lookFrom        = Vec3(6.0, 3.0, 6.0);
    cs.lookAt          = Vec3(0.0, 0.0, -2.0);
    cs.aspectRatio     = 1.0;
    cs.imageWidth      = 100;
    cs.verticalFOV     = 30.0;
    cs.focusDistance   = (cs.lookFrom - cs.lookAt).length();
    cs.defocusAngle    = 0.0;

    return Camera(cs);
}

int main() {
    return runScene("sphere_grid", buildCamera(),
                    {.samplesPerPixel = 50, .maxDepth = 50},
                    [] {
        Scene scene;
        buildScene(scene);
        return scene;
    });
}