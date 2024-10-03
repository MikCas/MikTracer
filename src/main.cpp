#include "../include/Utility.h"

#include <string>
#include <fstream>

#include "../include/Object.h"
#include "../include/ObjectList.h"
#include "../include/Sphere.h"

// double hit_sphere(const Vec3& center, double radius, const Ray& r){
//     // Vec3 oc = r.origin() - center;
//     // double a = dot(r.direction(), r.direction());
//     // double b = 2.0 * dot(oc, r.direction());
//     // double c = dot(oc, oc) - radius*radius;
//     // double discriminant = b*b - 4*a*c;

//     Vec3 oc = center - r.origin();

//     double a = r.direction().lengthSquared();
//     double h = dot(r.direction(), oc);
//     double c = oc.lengthSquared() - radius*radius;
//     double discriminant = h*h - a*c;

//     if (discriminant < 0) {
//         return -1.0;
//     } else {
//         return (h - std::sqrt(discriminant)) / a;
//     }
// }

// Vec3 rayColor(const Ray& r){
//     double t = hit_sphere(Vec3(0, 0, -1), 0.5, r);
//     if (t > 0.0){
//         Vec3 N = normalise(r.at(t) - Vec3(0, 0, -1));
//         return 0.5 * Vec3(N.x() + 1, N.y() + 1, N.z() + 1);
//     }

//     Vec3 unitDirection = normalise(r.direction());
//     double a = 0.5 * (unitDirection.y() + 1.0);
//     return (1.0 - a) * Vec3(1.0, 1.0, 1.0) + a * Vec3(0.5, 0.7, 1.0);
// }

Vec3 rayColor(const Ray& r, const Object& world){
    Hit hitRecord;
    if (world.hit(r, 0, infinity, hitRecord)){
        return 0.5 * (hitRecord.normal + Vec3(1, 1, 1));
    }

    Vec3 unitDirection = normalise(r.direction());
    double a = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - a) * Vec3(1.0, 1.0, 1.0) + a * Vec3(0.5, 0.7, 1.0);
}

void render(int imageWidth, int imageHeight, std::ofstream& outFile, Vec3 upperLeftPixel, Vec3 pixel_delta_u, Vec3 pixel_delta_v, Vec3 cameraOrigin, const Object& world){

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

            Vec3 pixelColor = rayColor(r, world);

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

    // World 
    ObjectList world;

    world.add(std::make_shared<Sphere>(Vec3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Vec3(0, -100.5, -1), 100));

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

    render(imageWidth, imageHeight, outFile, upperLeftPixel, pixel_delta_u, pixel_delta_v, cameraOrigin, world);

    outFile.close();

    return 0;
}