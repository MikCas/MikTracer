/* A minimalist render featuring matte, glass, and metal materials in a clean gallery setting*/

#include "Camera.h"
#include "Scene.h"
#include "Vec3.h"
#include "Material.h"
#include "SceneRunner.h"

#include <memory>   

void buildScene(Scene& scene) { 
    auto groundMat = std::make_shared<Lambertian>(Vec3(0.7, 0.6, 0.6));
    scene.world.add(std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, groundMat));

    auto mat1 = std::make_shared<Lambertian>(Vec3(0.9, 0.1, 0.1));
    auto mat2 = std::make_shared<Dielectric>(1.5);
    auto mat3 = std::make_shared<Metal>(Vec3(0.5, 0.05, 0.05), 0.1);

    scene.world.add(std::make_shared<Sphere>(Vec3(-1.1, 0.5, 0), 0.5, mat1));
    scene.world.add(std::make_shared<Sphere>(Vec3( 0.0,  0.5, 0), 0.5, mat2));
    scene.world.add(std::make_shared<Sphere>(Vec3( 0.0,  0.5, 0), -0.45, mat2));
    scene.world.add(std::make_shared<Sphere>(Vec3( 1.1, 0.5, 0), 0.5, mat3));
}

Camera buildCamera() {
    CameraSettings cs;
    cs.lookFrom = Vec3(0, 2, 4.0); 
    cs.lookAt   = Vec3(0, 0.5, 0); 
    cs.verticalFOV = 30.0; 
    cs.defocusAngle = 0.4; 
    cs.focusDistance = (cs.lookFrom - cs.lookAt).length();

    return Camera(cs);
}

int main() {
    return runScene("red", buildCamera(), 
                    {.samplesPerPixel = 200, .maxDepth = 50}, 
                    [] {
        Scene scene;
        buildScene(scene);
        return scene;
    });
}