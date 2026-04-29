#include "Camera.h"
#include "ImageBuffer.h"
#include "Scene.h"
#include "Vec3.h"

// Classic "Ray Tracing in One Weekend" hero shot:
//   - large diffuse ground
//   - matte sphere on the left
//   - glass sphere in the centre
//   - polished metal sphere on the right

void buildScene(Scene& scene) {
    // Ground
    scene.addSphere(Vec3(0.0, -100.5, -1.0), 100.0,
                    scene.lambertian(Vec3(0.8, 0.8, 0.0)));

    // Left — matte red
    scene.addSphere(Vec3(-1.0, 0.0, -1.0), 0.5,
                    scene.lambertian(Vec3(0.7, 0.3, 0.3)));

    // Centre — glass
    scene.addSphere(Vec3(0.0, 0.0, -1.0), 0.5,
                    scene.dielectric(1.5));

    // Right — polished metal
    scene.addSphere(Vec3(1.0, 0.0, -1.0), 0.5,
                    scene.metal(Vec3(0.8, 0.8, 0.8), 0.0));
}

Camera buildCamera() {
    Vec3 lookFrom(0.0, 0.5, 2.5);
    Vec3 lookAt(0.0, 0.0, -1.0);

    double aspectRatio     = 16.0 / 9.0;
    int    imageWidth      = 480;
    int    samplesPerPixel = 50;
    int    maxDepth        = 50;
    double verticalFOV     = 30.0;
    double focusDistance   = (lookFrom - lookAt).length();
    double defocusAngle    = 0.0;

    return Camera(lookFrom, lookAt, aspectRatio, imageWidth,
                  samplesPerPixel, maxDepth, verticalFOV,
                  focusDistance, defocusAngle);
}

int main() {
    Scene scene(buildCamera());
    buildScene(scene);

    ImageBuffer image(scene.camera().imageWidth(), scene.camera().imageHeight());
    scene.camera().render(image, scene.world());

    image.writePNG("renders/three_spheres.png");
    return 0;
}
