#include "Utility.h"

#include <string>

#include "Vec3.h"
#include "Ray.h"
#include "Interval.h"
#include "Camera.h"
#include "Object.h"
#include "Material.h"
#include "ImageBuffer.h"
#include "Scene.h"

// Colors
const Vec3 RED = Vec3(0.8, 0.0, 0.0);
const Vec3 GREEN = Vec3(0.0, 1.0, 0.0);
const Vec3 BLUE = Vec3(0.0, 0.0, 1.0);
const Vec3 WHITE = Vec3(1.0, 1.0, 1.0);
const Vec3 BLACK = Vec3(0.0, 0.0, 0.0);
const Vec3 LIGHT_GRAY = Vec3(0.5, 0.5, 0.5);
const Vec3 DARK_GRAY = Vec3(0.9, 0.9, 0.9);

std::shared_ptr<Sphere> createRandomSphere(Vec3 position, double maxRadius){

    double radius = randomDouble(maxRadius-0.2, maxRadius);

    Vec3 color = lerp(randomDouble(), WHITE, BLACK);

    std::shared_ptr<Material> material = std::make_shared<Metal>(color, randomDouble());
    // std::shared_ptr<Material> mat = std::make_shared<Lambertian>(color);

    auto sphere = std::make_shared<Sphere>(position, radius, material);
    return sphere;
}

ObjectList createWorld(){
    ObjectList world;

    // Settings
    double radius = 0.5;
    double groundRadius = 100.0;

    int numCols = 6;
    int numRows = 10;
    double offset = numCols*radius; // used to position the spheres in the center
    
    // Ground
    auto groundMat = std::make_shared<Lambertian>(WHITE);
    Vec3 groundPos = Vec3(0.0, -groundRadius - radius, -1);
    world.add(std::make_shared<Sphere>(groundPos, groundRadius, groundMat));

    for(int i = 0; i < numCols; ++i){
        double xPos = 2*radius*i - offset + radius;

        for(int j = 0; j < numRows; ++j){
            double zPos = -2*j*radius + 2*radius;
            Vec3 pos = Vec3(xPos, 0.0, zPos);
            world.add(createRandomSphere(pos, radius));
        }
    }

    return world;
}

Camera defaultCamera(){

    // Image Settings
    double aspectRatio = 1.0; 
    int imageWidth = 100;
    int samplesPerPixel = 100;

    Vec3 lookFrom(6, 3, 6);
    Vec3 lookAt(0, 0, -2);
    double verticalFOV = 30.0;
    int maxDepth = 50;

    Vec3 viewDirection = lookFrom - lookAt;
    double focusDistance = viewDirection.length();
    double defocusAngle = 0.0;

    return Camera(lookFrom, lookAt, aspectRatio, imageWidth, samplesPerPixel, maxDepth, verticalFOV, focusDistance, defocusAngle);
}

int main() {

    Camera camera = defaultCamera();
    Scene scene(camera);

    scene.addSphere(Vec3(0, -100.5, -1), 100, scene.lambertian(Vec3(0.8, 0.8, 0.0)));
    scene.addSphere(Vec3(0,    0,   -1), 0.5, scene.lambertian(Vec3(0.7, 0.3, 0.3)));
    scene.addSphere(Vec3(1,    0,   -1), 0.5, scene.metal(Vec3(0.8, 0.8, 0.8), 0.0));

    ImageBuffer image(camera.imageWidth(), camera.imageHeight());
    


    ObjectList world = createWorld();

    ImageBuffer img(100, 100);
    camera.render(img, world);
    img.writePPM("image.ppm");

    return 0;
}


// #include "ImageBuffer.h"

// int main() {

//     ImageBuffer img(256, 256);

//     for (int j = 0; j < 256; j++){
//         for (int i = 0; i < 256; i++) {
//             img.setPixel(i, j, Vec3(i / 255.0, j / 255.0, 0.25));
//         }
//     }

//     img.writePPM("test_ppm.ppm");
//     img.writePNG("test_png.png");

//     return 0;
// }