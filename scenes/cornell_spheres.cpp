#include "Camera.h"
#include "ImageBuffer.h"
#include "Scene.h"
#include "Vec3.h"
#include "Material.h"
#include <memory>  

// Depth-of-field showcase: a row of spheres receding into the distance.
// Camera focuses on the front sphere; the rest progressively blur.

void buildScene(Scene& scene) {
    // Ground
    scene.addSphere(Vec3(0.0, -100.5, -1.0), 100.0,
                    std::make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5)));

    // Glass sphere (in focus)
    scene.addSphere(Vec3(0.0, 0.0, -1.0), 0.5,
                    std::make_shared<Dielectric>(1.5));

    // Receding spheres along negative Z — alternating materials
    scene.addSphere(Vec3(-0.6, 0.0, -2.0), 0.4,
                    std::make_shared<Lambertian>(Vec3(0.8, 0.2, 0.2)));
    scene.addSphere(Vec3( 0.7, 0.0, -2.5), 0.4,
                    std::make_shared<Metal>(Vec3(0.9, 0.7, 0.3), 0.05));
    scene.addSphere(Vec3(-0.8, 0.0, -3.5), 0.4,
                    std::make_shared<Lambertian>(Vec3(0.2, 0.4, 0.8)));
    scene.addSphere(Vec3( 1.0, 0.0, -4.5), 0.4,
                    std::make_shared<Metal>(Vec3(0.7, 0.7, 0.7), 0.2));
    scene.addSphere(Vec3(-1.2, 0.0, -5.5), 0.4,
                    std::make_shared<Lambertian>(Vec3(0.3, 0.7, 0.3)));
}

Camera buildCamera() {
    CameraSettings cs;
    cs.lookFrom        = Vec3(0.0, 0.6, 1.5);
    cs.lookAt          = Vec3(0.0, 0.0, -1.0);
    cs.aspectRatio     = 16.0 / 9.0;
    cs.imageWidth      = 480;
    cs.samplesPerPixel = 100;
    cs.maxDepth        = 50;
    cs.verticalFOV     = 35.0;

    cs.focusDistance   = (cs.lookFrom - cs.lookAt).length(); 
    cs.defocusAngle    = 4.0;

    return Camera(cs);
}

int main() {
    Scene scene(buildCamera());
    buildScene(scene);

    ImageBuffer image(scene.camera.imageWidth(), scene.camera.imageHeight());
    scene.camera.render(image, scene.world);

    image.writePNG("renders/cornell_spheres.png");
    return 0;
}
