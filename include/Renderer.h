#pragma once

#include "Camera.h"
#include "Scene.h"
#include "ImageBuffer.h"
#include "Ray.h"
#include "Vec3.h"

struct RendererSettings {
    int samplesPerPixel = 50;  // Number of samples per pixel - for anti-aliasing
    int maxDepth        = 50;  // Maximum number of bounces for a ray  
};

class Renderer {
private:

    int m_samplesPerPixel;
    int m_maxDepth;

    // Derived variables in constructor
    double m_pixelSampleScale; 

    Vec3 rayColor(const Ray& r, int depth, const Object& world) const;

public:
    explicit Renderer(const RendererSettings& settings);
    void render(const Scene& scene, const Camera& camera, ImageBuffer& image) const;
};