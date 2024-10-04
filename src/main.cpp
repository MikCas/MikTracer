#include "../include/Utility.h"

#include <string>

#include "../include/Vec3.h"
#include "../include/Ray.h"
#include "../include/Interval.h"
#include "../include/Camera.h"
#include "../include/Object.h"
#include "../include/ObjectList.h"
#include "../include/Sphere.h"

int main() {

    // Image 
    double aspectRatio = 16.0 / 9.0;
    // double aspectRatio = 9.0 / 16.0;
    int imageWidth = 400;
    Camera camera(aspectRatio, imageWidth, 1, 2, 100, 50);

    // World 
    ObjectList world;

    world.add(std::make_shared<Sphere>(Vec3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Vec3(0, -100.5, -1), 100));

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