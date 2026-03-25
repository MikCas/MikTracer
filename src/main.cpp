#include "../include/Utility.h"

#include <string>

#include "../include/Vec3.h"
#include "../include/Ray.h"
#include "../include/Interval.h"
#include "../include/Camera.h"
#include "../include/Object.h"
#include "../include/Material.h"

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

Camera A5Camera(){

    // Image Settings
    double aspectRatio = 148.0 / 210.0; // A5 aspect ratio
    int imageWidth = 1522;

    // Camera Settings
    int samplesPerPixel = 500;
    int maxDepth = 50;
    double verticalFOV = 20.0;

    Vec3 lookFrom(-20, 20.0, 10.0);
    Vec3 lookAt(0, 0, -1.0);
    Vec3 viewDirection = lookFrom - lookAt;

    double focusDistance = viewDirection.length();
    double defocusAngle = 0.0;

    return Camera(lookFrom, lookAt, aspectRatio, imageWidth, samplesPerPixel, maxDepth, verticalFOV, focusDistance, defocusAngle);
}

Camera quickRender(){
    
    // Image Settings
    // double aspectRatio = 148.0 / 210.0;
    double aspectRatio = 1.0;
    int imageWidth = 200;

    // Camera Settings
    int samplesPerPixel = 10;
    int maxDepth = 50;
    double verticalFOV = 20.0;

    Vec3 lookFrom(-20, 20.0, 10.0);
    Vec3 lookAt(0, 0, -1.0);
    Vec3 viewDirection = lookFrom - lookAt;

    double focusDistance = viewDirection.length();
    double defocusAngle = 0.0;

    return Camera(lookFrom, lookAt, aspectRatio, imageWidth, samplesPerPixel, maxDepth, verticalFOV, focusDistance, defocusAngle);
}

int main() {

    // Camera
    Camera camera = quickRender();

    // World 
    ObjectList world = createWorld();

    // Render
    std::string outputFileName = "image.ppm";
    std::ofstream outFile(outputFileName);

    if (!outFile) {
        std::cerr << "Error: Could not open the file for writing :( \n";
        return 1;
    }

    camera.render(outFile, world, WHITE, DARK_GRAY);

    outFile.close();

    return 0;
}