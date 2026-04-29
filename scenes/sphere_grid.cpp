#include "Camera.h"
#include "ImageBuffer.h"
#include "Scene.h"
#include "Utility.h"
#include "Vec3.h"

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
    scene.addSphere(groundPos, groundRadius, scene.lambertian(WHITE));

    // Grid of metal spheres
    for (int i = 0; i < numCols; ++i) {
        double xPos = 2 * radius * i - offset + radius;
        for (int j = 0; j < numRows; ++j) {
            double zPos = -2 * j * radius + 2 * radius;
            Vec3 pos(xPos, 0.0, zPos);

            double r = randomDouble(radius - 0.2, radius);
            Vec3 albedo = lerp(randomDouble(), WHITE, BLACK);
            scene.addSphere(pos, r, scene.metal(albedo, randomDouble()));
        }
    }
}

Camera buildCamera() {
    Vec3 lookFrom(6, 3, 6);
    Vec3 lookAt(0, 0, -2);

    double aspectRatio    = 1.0;
    int    imageWidth     = 400;
    int    samplesPerPixel = 50;
    int    maxDepth       = 50;
    double verticalFOV    = 30.0;
    double focusDistance  = (lookFrom - lookAt).length();
    double defocusAngle   = 0.0;

    return Camera(lookFrom, lookAt, aspectRatio, imageWidth,
                  samplesPerPixel, maxDepth, verticalFOV,
                  focusDistance, defocusAngle);
}

int main() {
    Scene scene(buildCamera());
    buildScene(scene);

    ImageBuffer image(scene.camera().imageWidth(), scene.camera().imageHeight());
    scene.camera().render(image, scene.world());

    image.writePNG("renders/sphere_grid.png");
    return 0;
}
