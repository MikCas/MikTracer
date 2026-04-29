#include "Camera.h"
#include "Scene.h"
#include "Vec3.h"
#include "Material.h"
#include "SceneRunner.h"
#include "Utility.h"

#include <memory>   

std::shared_ptr<Material> randomMaterial(Vec3 baseColor, double spread, int type) {
    Vec3 color = baseColor + Vec3::random(-spread, spread);   // Jitter the color
    color = color.clamp(0, 1);

    switch(type) {
        case 0: // Matte
            return std::make_shared<Lambertian>(color);
            
        case 1: // Metal
            return std::make_shared<Metal>(color, randomDouble(0, spread));
            
        case 2: // Dielectric - vary refractive index slightly
            return std::make_shared<Dielectric>(1.5 + randomDouble(-0.2, 0.2));

        default:
            return std::make_shared<Lambertian>(color);
    }
}

Vec3 randomPosition(Vec3 center, Vec3 spread) {
    return center + Vec3::random(-spread, spread);
}

double randomRadius(double baseRadius, double spread) {
    // Ensure radius is above 0
    double r = baseRadius + randomDouble(-spread, spread);
    return (r < 0.05) ? 0.05 : r;
}

void addCluster(Scene& scene, int n, Vec3 center, Vec3 centerSpread, 
                Vec3 color, double colorSpread, int type, 
                double baseRadius, double radiusSpread) {
    
    for (int i = 0; i < n; i++) {
        Vec3 pos = randomPosition(center, centerSpread);
        auto mat = randomMaterial(color, colorSpread, type);
        double r = randomRadius(baseRadius, radiusSpread);
        
        scene.world.add(std::make_shared<Sphere>(pos, r, mat));
    }
}

void buildScene(Scene& scene) {
    // Ground
    auto ground_mat = std::make_shared<Metal>(Vec3(0.02, 0.02, 0.05), 0.05);
    scene.world.add(std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, ground_mat));

    // Set of colours
    Vec3 colors[] = {
        Vec3(0.1, 0.3, 0.8), // Deep Blue
        Vec3(0.2, 0.6, 1.0), // Sky Blue
        Vec3(0.4, 0.9, 1.0), // Cyan
        Vec3(0.8, 0.8, 0.9)  // Silver
    };

    // Procedural Cluster Generation
    for (int k = 0; k < 50; k++) {

        const double stageSize = 15.0;

        // Random cluster metadata
        int n             = static_cast<int>(randomDouble(1, 40));

        Vec3 pos = Vec3(randomDouble(-stageSize, stageSize), 
                                randomDouble(0, 3.0), // Keep them relatively low
                                randomDouble(-stageSize, stageSize));
        Vec3 posSpread    = Vec3::random(0.1, 1.0);

        Vec3 baseColor    = colors[static_cast<int>(randomDouble(0, 3.99))];
        double colorVar   = randomDouble(0.05, 0.2);
        int matType       = static_cast<int>(randomDouble(0, 2.99));

        double baseR      = randomDouble(0, 0.5);
        double rSpread   = baseR * randomDouble(); // Variation relative to size

        addCluster(scene, n, pos, posSpread, baseColor, colorVar, matType, baseR, rSpread);
    }
}

Camera buildCamera() {
    CameraSettings cs;
    cs.lookFrom = Vec3(45, 12, 45); 
    cs.lookAt   = Vec3(0, 1, 0); 
    
    cs.verticalFOV = 10.0; 
    
    cs.defocusAngle = 0.2; 
    cs.focusDistance = (cs.lookFrom - cs.lookAt).length();
    
    cs.imageWidth = 600; // Keep it low for testing, 1200 for final
    cs.aspectRatio = 16.0 / 9.0;
    return Camera(cs);
}

int main() {
    return runScene("blue", buildCamera(), 
                    {.samplesPerPixel = 1, .maxDepth = 2}, 
                    [] {
        Scene scene;
        buildScene(scene);
        return scene;
    });
}