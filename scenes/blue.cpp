/* A generative scene utilising algorithmic techniques and rendomisation to create generative clusters of spheres*/

#include "Camera.h"
#include "Scene.h"
#include "Vec3.h"
#include "Material.h"
#include "SceneRunner.h"
#include "Utility.h"

#include <memory>  
#include <array>

enum class MaterialType {
    Matte, 
    Metal, 
    Dielectric
};

struct ClusterParams {
    int            count;
    Vec3           center;
    Vec3           positionSpread;
    Vec3           baseColor;
    double         colorSpread;
    MaterialType   material;
    double         baseRadius;
    double         radiusSpread;
};

std::shared_ptr<Material> randomMaterial(Vec3 baseColor, double spread, MaterialType material){
    // Multiply by a scalar to to vary the brightness and keep hue the same
    double brightness = randomDouble(1.0 - spread, 1.0 + spread);
    Vec3 color = (baseColor * brightness).clamp(0.0, 1.0); 

    switch(material) {
        case MaterialType::Matte:
            return std::make_shared<Lambertian>(color);
        case MaterialType::Metal:
            return std::make_shared<Metal>(color, randomDouble(0, spread * 0.5));
        case MaterialType::Dielectric:
            return std::make_shared<Dielectric>(1.5 + randomDouble(-0.1, 0.1));
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

void addCluster(Scene& scene, const ClusterParams& c) {
    
    for (int i = 0; i < c.count; i++) {
        double r = randomRadius(c.baseRadius, c.radiusSpread);

        Vec3 offset = Vec3::random() * 2.0 - Vec3(1.0, 1.0, 1.0);
        offset = normalise(offset) * randomDouble(0.0, c.positionSpread.x());
        Vec3 p = c.center + offset;
        p[1] = std::max(p.y(), r);
       
        auto m = randomMaterial(c.baseColor, c.colorSpread, c.material);
        
        scene.world.add(std::make_shared<Sphere>(p, r, m));
    }
}

void buildScene(Scene& scene) {
    
    const double stageSize = 8.0;
    const int numClusters = 60;
    const int maxClusterCount = 30;
    const double maxBaseRadius = 0.2;

    const std::array<Vec3, 4> palette = {
        Vec3(0.1, 0.3, 0.8), // Deep Blue
        Vec3(0.2, 0.6, 1.0), // Sky Blue
        Vec3(0.4, 0.9, 1.0), // Cyan
        Vec3(0.8, 0.8, 0.9)  // Silver
    };

    // Ground
    auto groundMat = std::make_shared<Lambertian>(Vec3(0.8, 0.8, 0.8));
    scene.world.add(std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, groundMat));

    // Procedural Cluster Generation
    for (int k = 0; k < numClusters; ++k) {

        const double baseRadius = randomDouble(0.01, maxBaseRadius);

        int roll = randomInt(0, 9);
        MaterialType matType = (roll < 5) ? MaterialType::Matte
                            : (roll < 9) ? MaterialType::Metal
                                        : MaterialType::Dielectric;

        double xBias = randomDouble(-1.0, 1.0);
        double zBias = randomDouble(-1.0, 1.0);

        Vec3 clusterCenter = Vec3(
            xBias * stageSize,
            baseRadius, // Start the cluster center on the ground
            zBias * stageSize
        );

        addCluster(scene, ClusterParams{
            .count          = randomInt(1, maxClusterCount),
            .center         = clusterCenter,
            .positionSpread = Vec3(1, 1, 1) * baseRadius * 5,
            .baseColor      = palette[randomInt(0, palette.size() - 1)],
            .colorSpread    = 0.3,
            .material       = matType,
            .baseRadius     = baseRadius,
            .radiusSpread   = baseRadius * 1, // relative to size
        });
    }
}

Camera buildCamera() {
    CameraSettings cs;
    cs.lookFrom      = Vec3(60, 60, 60);   
    cs.lookAt        = Vec3(0, 0, 0); 
    cs.verticalFOV   = 5.0;                
    cs.defocusAngle  = 0.0;                
    cs.focusDistance = (cs.lookFrom - cs.lookAt).length();
    cs.imageWidth    = 1920;
    cs.aspectRatio   = 16.0 / 9.0;
    return Camera(cs);
}

int main() {
    return runScene("blue", buildCamera(), 
                    {.samplesPerPixel = 500, .maxDepth = 50}, 
                    [] {
        Scene scene;
        buildScene(scene);
        return scene;
    });
}