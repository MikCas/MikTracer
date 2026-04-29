#include "Camera.h"
#include "ImageBuffer.h"
#include "Scene.h"
#include "Vec3.h"

// Depth-of-field showcase: a row of spheres receding into the distance.
// Camera focuses on the front sphere; the rest progressively blur.

void buildScene(Scene& scene) {
    // Ground
    scene.addSphere(Vec3(0.0, -100.5, -1.0), 100.0,
                    scene.lambertian(Vec3(0.5, 0.5, 0.5)));

    // Hero glass sphere (in focus)
    scene.addSphere(Vec3(0.0, 0.0, -1.0), 0.5,
                    scene.dielectric(1.5));

    // Receding spheres along negative Z — alternating materials
    scene.addSphere(Vec3(-0.6, 0.0, -2.0), 0.4,
                    scene.lambertian(Vec3(0.8, 0.2, 0.2)));
    scene.addSphere(Vec3( 0.7, 0.0, -2.5), 0.4,
                    scene.metal(Vec3(0.9, 0.7, 0.3), 0.05));
    scene.addSphere(Vec3(-0.8, 0.0, -3.5), 0.4,
                    scene.lambertian(Vec3(0.2, 0.4, 0.8)));
    scene.addSphere(Vec3( 1.0, 0.0, -4.5), 0.4,
                    scene.metal(Vec3(0.7, 0.7, 0.7), 0.2));
    scene.addSphere(Vec3(-1.2, 0.0, -5.5), 0.4,
                    scene.lambertian(Vec3(0.3, 0.7, 0.3)));
}

Camera buildCamera() {
    Vec3 lookFrom(0.0, 0.6, 1.5);
    Vec3 lookAt(0.0, 0.0, -1.0);

    double aspectRatio     = 16.0 / 9.0;
    int    imageWidth      = 480;
    int    samplesPerPixel = 100;
    int    maxDepth        = 50;
    double verticalFOV     = 35.0;
    double focusDistance   = (lookFrom - lookAt).length();
    double defocusAngle    = 4.0;     // <-- the blur effect

    return Camera(lookFrom, lookAt, aspectRatio, imageWidth,
                  samplesPerPixel, maxDepth, verticalFOV,
                  focusDistance, defocusAngle);
}

int main() {
    Scene scene(buildCamera());
    buildScene(scene);

    ImageBuffer image(scene.camera.imageWidth(), scene.camera.imageHeight());
    scene.camera.render(image, scene.world);

    image.writePNG("renders/cornell_spheres.png");
    return 0;
}
