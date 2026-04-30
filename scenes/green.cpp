/* A procedural render featuring stacks of spheres in a monochrome green palette. */

#include "Camera.h"
#include "Scene.h"
#include "Vec3.h"
#include "Material.h"
#include "SceneRunner.h"
#include "Utility.h"

#include <memory>

struct StackConfig {
    int    numStacks      = 25;
    int    minHeight      = 3;
    int    maxHeight      = 15;
    double minBaseRadius  = 0.1;
    double maxBaseRadius  = 0.35;
    double rangeX         = 5.0;
    double rangeZMin      = -5.0;
    double rangeZMax      = 2.0;
    double radiusDecay    = 0.9;
    double minStopRadius  = 0.02;
    double jitterPos      = 0.03;
    Vec3   groundColor    = Vec3(0.8, 0.8, 0.8);
    Vec3   colorMin       = Vec3(0.05, 0.4, 0.05);
    Vec3   colorMax       = Vec3(0.2, 0.8, 0.2);
};

void generateStack(Scene& scene, int n, Vec3 pos, double radius, const StackConfig& cfg) {
    for (int i = 0; i < n && radius > cfg.minStopRadius; ++i, radius *= cfg.radiusDecay) {
        double r = radius * randomDouble(0.8, 1.2);
        
        Vec3 color(
            randomDouble(cfg.colorMin.x(), cfg.colorMax.x()),
            randomDouble(cfg.colorMin.y(), cfg.colorMax.y()),
            randomDouble(cfg.colorMin.z(), cfg.colorMax.z())
        );
        
        Vec3 jitter(randomDouble(-cfg.jitterPos, cfg.jitterPos), 0, randomDouble(-cfg.jitterPos, cfg.jitterPos));

        pos[1] += r; 
        scene.world.add(std::make_shared<Sphere>(pos + jitter, r, std::make_shared<Lambertian>(color)));
        pos[1] += r; 
    }
}

void buildScene(Scene& scene) {
    StackConfig cfg; // Initialize our "recipe"

    scene.world.add(std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, std::make_shared<Lambertian>(cfg.groundColor)));

    for (int i = 0; i < cfg.numStacks; ++i) {
        int height    = randomInt(cfg.minHeight, cfg.maxHeight);
        double radius = randomDouble(cfg.minBaseRadius, cfg.maxBaseRadius);
        double x      = randomDouble(-cfg.rangeX, cfg.rangeX);
        double z      = randomDouble(cfg.rangeZMin, cfg.rangeZMax);

        generateStack(scene, height, Vec3(x, 0, z), radius, cfg);
    }
}

Camera buildCamera() {
    CameraSettings cs;
    // Moved lookFrom slightly wider and lookAt to the true center of the cluster (-1.5 on Z)
    cs.lookFrom = Vec3(10, 6, 10);
    cs.lookAt   = Vec3(0, 1, -1.5); 
    
    cs.verticalFOV = 20.0;
    cs.defocusAngle = 0.2; // Reduced slightly to keep more stacks in focus
    cs.focusDistance = (cs.lookFrom - cs.lookAt).length();
 
    cs.imageWidth    = 600;
    
    return Camera(cs);
}

int main() {
    return runScene("green", buildCamera(), 
                    {.samplesPerPixel = 200, .maxDepth = 50}, 
                    [] {
        Scene scene;
        buildScene(scene);
        return scene;
    });
}
