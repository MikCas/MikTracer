#include "Camera.h"
#include "Scene.h"
#include "Vec3.h"
#include "Material.h"
#include "SceneRunner.h"
#include "Utility.h"

#include <memory>   

std::shared_ptr<Material> randomMaterial(Vec3 baseColor, double spread) {
    Vec3 color = baseColor + Vec3::random(-spread, spread);
    color.clamp(0, 1);
}


void addCluster(Scene& scene, int n, Vec3 center, Vec3 centerSpread, Vec3 color, double colorSpread) {
    Vec3 min = center - centerSpread;
    Vec3 max = center + centerSpread;
    
    for(int i = 0; i < n; i++){
        Vec3 pos = center + Vec3::random(min, max);
        double radius = randomDouble(0.1, 1.0);
        auto jitter = Vec3::random(-colorSpread, colorSpread);
        Vec3 finalColor = color + jitter;
        for(int i=0; i<3; i++) {
            if (finalColor[i] < 0) finalColor[i] = 0;
            if (finalColor[i] > 1) finalColor[i] = 1;
        }

        if (randomDouble() < 0.2) {
            auto mat = std::make_shared<Metal>(finalColor, randomDouble(0, 0.3));
        }
        else {
            auto mat = std::make_shared<Lambertian>(finalColor);
        }

        scene.world.add(std::make_shared<Sphere>(pos, radius, mat));
    }
}

void buildScene(Scene& scene) {
    // Ground
    auto ground_mat = std::make_shared<Metal>(Vec3(0.1, 0.1, 0.2), 0.15);
    scene.world.add(std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, ground_mat));

    // Set of colours
    Vec3 colors[] = {
        Vec3(0.1, 0.3, 0.8), // Deep Blue
        Vec3(0.2, 0.6, 1.0), // Sky Blue
        Vec3(0.4, 0.9, 1.0), // Cyan
        Vec3(0.8, 0.8, 0.9)  // Silver
    };

    // Procedural Cluster Generation
    for(int k = 0; k < 20; k++){
        // Pick a random center for the cluster
        Vec3 pos = Vec3::random(-10, 10);
        Vec3 posSpread = Vec3::random(0.1, 5);

        Vec3 color = colors[static_cast<int>(randomDouble(0, 3.99))];
        double colorSpread = randomDouble(0.05, 0.25); 


        addCluster(scene, 20, pos, posSpread, color, colorSpread);
    }
}

Camera buildCamera() {
    CameraSettings cs;
    cs.lookFrom = Vec3(12, 100, 12);
    cs.lookAt   = Vec3(0, 0, -2);
    cs.verticalFOV = 25.0;
    
    cs.defocusAngle = 0.3;
    cs.focusDistance = 22.0; // The distance from (12,15,12) to (0,0,0) is roughly 22 
    
    cs.imageWidth = 200;
    cs.aspectRatio = 16.0 / 9.0;
    return Camera(cs);
}

int main() {
    return runScene("blue", buildCamera(), 
                    {.samplesPerPixel = 50, .maxDepth = 20}, 
                    [] {
        Scene scene;
        buildScene(scene);
        return scene;
    });
}