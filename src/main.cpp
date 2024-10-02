#include <iostream>
#include <string>
#include <fstream>

#include "../include/Vec3.h"
#include "../include/Ray.h"
#include "../include/Color.h"

Vec3 rayColor(const Ray& r){
    Vec3 unitDirection = normalise(r.direction());
    double t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}

void render(int imageWidth, int imageHeight, std::ofstream& outFile, Vec3 upperLeftPixel, Vec3 pixel_delta_u, Vec3 pixel_delta_v, Vec3 cameraOrigin){

    outFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for(int j = 0; j < imageHeight; j++){
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << '\n' << std::flush;
        for(int i = 0; i < imageWidth; i++){
            
            Vec3 pixelCenter = upperLeftPixel + (i * pixel_delta_u) + (j * pixel_delta_v);
            Vec3 rayDirection = pixelCenter - cameraOrigin;
            Ray r(cameraOrigin, rayDirection);

            // auto r = double(i) / (imageWidth - 1);
            // auto g = double(j) / (imageHeight - 1);
            // auto b = 0.0;

            Vec3 pixelColor = rayColor(r);

            writeColor(outFile, pixelColor);
        }
    }
    std::clog << "\rDone.                 \n";
}

int main() {

    // Image 
    double aspectRatio = 16.0 / 9.0;
    int imageWidth = 400;
    int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    imageHeight = std::max(imageHeight, 1);

    // Camera 
    double focalLength = 1.0;
    double viewportHeight = 2.0;
    double viewportWidth = viewportHeight * (static_cast<double>(imageWidth)/imageHeight);
    Vec3 cameraOrigin = Vec3(0, 0, 0);

    // Vectors across horizontal and down vertical viewport edges
    Vec3 viewportU = Vec3(viewportWidth, 0, 0);
    Vec3 viewportV = Vec3(0, -viewportHeight, 0);

    // Delta vectors from pixel to pixel
    Vec3 pixel_delta_u = viewportU / imageWidth ;
    Vec3 pixel_delta_v = viewportV / imageHeight;

    // Location of upper left pixel
    Vec3 viewportUpperLeft = cameraOrigin - Vec3(0, 0, focalLength) - viewportU/2 - viewportV/2;
    Vec3 upperLeftPixel = viewportUpperLeft + 0.5*(pixel_delta_u + pixel_delta_v);

    // Render
    std::string outputFileName = "../image.ppm";

    std::ofstream outFile(outputFileName);

    if (!outFile) {
        std::cerr << "Error: Could not open the file for writing :( \n";
        return 1;
    }

    render(imageWidth, imageHeight, outFile, upperLeftPixel, pixel_delta_u, pixel_delta_v, cameraOrigin);

    outFile.close();

    return 0;
}