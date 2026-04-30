// Jittered grid of red spheres

#include "Camera.h"
#include "Scene.h"
#include "Vec3.h"
#include "Material.h"
#include "SceneRunner.h"
#include <memory>
#include <array>

enum class MaterialType { Matte, Metal, Dielectric };

struct GridParams {
    int    halfExtent;      // grid spans [-halfExtent, halfExtent]
    double spacing;
    double baseRadius;
    double radiusSpread;
    double positionJitter;
    double colorSpread;
    Vec3   groundColor;
    std::array<Vec3, 4> palette;
};

std::shared_ptr<Material> randomMaterial(Vec3 baseColor, double spread, MaterialType type) {
    double brightness = randomDouble(1.0 - spread, 1.0 + spread);
    Vec3 color = (baseColor * brightness).clamp(0.0, 1.0);

    switch (type) {
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

MaterialType randomMaterialType() {
    int roll = randomInt(0, 9);
    if (roll < 5) return MaterialType::Matte;
    if (roll < 9) return MaterialType::Metal;
    return MaterialType::Dielectric;
}

void buildScene(Scene& scene) {

    const GridParams grid {
        .halfExtent    = 6,
        .spacing       = 1.1,
        .baseRadius    = 0.4,
        .radiusSpread  = 0.15,
        .positionJitter = 0.2,
        .colorSpread   = 0.3,
        .groundColor   = Vec3(0.7, 0.6, 0.6),
        .palette       = {
            Vec3(0.9, 0.1, 0.1),   // Bright Red
            Vec3(0.7, 0.05, 0.05), // Dark Red
            Vec3(0.95, 0.3, 0.2),  // Orange-Red
            Vec3(0.5, 0.05, 0.05)  // Maroon
        }
    };

    // Ground plane
    auto groundMat = std::make_shared<Lambertian>(grid.groundColor);
    scene.world.add(std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, groundMat));

    // Sphere grid
    for (int i = -grid.halfExtent; i <= grid.halfExtent; ++i) {
        for (int j = -grid.halfExtent; j <= grid.halfExtent; ++j) {
            double r = grid.baseRadius + randomDouble(-grid.radiusSpread, grid.radiusSpread);
            Vec3 center(
                i * grid.spacing + randomDouble(-grid.positionJitter, grid.positionJitter),
                r,
                j * grid.spacing + randomDouble(-grid.positionJitter, grid.positionJitter)
            );

            Vec3 baseColor = grid.palette[randomInt(0, grid.palette.size() - 1)];
            auto mat = randomMaterial(baseColor, grid.colorSpread, randomMaterialType());
            scene.world.add(std::make_shared<Sphere>(center, r, mat));
        }
    }
}

Camera buildCamera() {
    CameraSettings cs;
    cs.lookFrom     = Vec3(0, 40, 0.01);
    cs.lookAt       = Vec3(0, 0, 0);
    cs.verticalFOV  = 12.0;
    cs.defocusAngle = 0.0;
    cs.focusDistance = 40.0;
    cs.imageWidth   = 600;
    return Camera(cs);
}

int main() {
    return runScene("red", buildCamera(),
                    {.samplesPerPixel = 500, .maxDepth = 50},
                    [] {
        Scene scene;
        buildScene(scene);
        return scene;
    });
}