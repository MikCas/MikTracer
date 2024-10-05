#include "../include/Utility.h"

#include <string>

#include "../include/Vec3.h"
#include "../include/Ray.h"
#include "../include/Interval.h"
#include "../include/Camera.h"
#include "../include/Object.h"
#include "../include/Material.h"

ObjectList createWorld(){
    ObjectList world;

    auto material_ground = std::make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Vec3(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<Metal>(Vec3(0.8, 0.8, 0.8));
    auto material_right  = std::make_shared<Metal>(Vec3(0.8, 0.6, 0.2));

    world.add(std::make_shared<Sphere>(Vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<Sphere>(Vec3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(std::make_shared<Sphere>(Vec3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<Sphere>(Vec3( 1.0,    0.0, -1.0),   0.5, material_right));

    return world;
}

int main() {

    // Image 
    double aspectRatio = 16.0 / 9.0;
    int imageWidth = 400;
    Camera camera(aspectRatio, imageWidth, 1, 2, 100, 50);

    // World 
    ObjectList world = createWorld();

    // Render
    std::string outputFileName = "../image.ppm";
    std::ofstream outFile(outputFileName);

    if (!outFile) {
        std::cerr << "Error: Could not open the file for writing :( \n";
        return 1;
    }

    camera.render(outFile, world);

    outFile.close();

    return 0;
}