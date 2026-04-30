#include "Renderer.h"
#include <iostream>

Renderer::Renderer(const RendererSettings& settings)
    : m_samplesPerPixel(settings.samplesPerPixel),
      m_maxDepth(settings.maxDepth) {
    
    // Anti-aliasing
    m_pixelSampleScale  = 1.0 / m_samplesPerPixel;

}

void Renderer::render(const Scene& scene, const Camera& camera, ImageBuffer& image) const {
    for (int j = 0; j < camera.imageHeight(); ++j) {
        // Progress indicator fix: \r returns to the start of the line, spaces overwrite old characters
        std::clog << "\rScanlines remaining: " << (camera.imageHeight() - j) << "   " << std::flush;
        
        for (int i = 0; i < camera.imageWidth(); ++i) {
            Vec3 pixelColor(0, 0, 0);
            for (int sample = 0; sample < m_samplesPerPixel; ++sample) {
                Ray r = camera.getRay(i, j); 
                pixelColor += rayColor(r, m_maxDepth, scene.world);
            }
            
            pixelColor *= m_pixelSampleScale;
            
            image.setPixel(i, j, pixelColor);
        }
    }
    std::clog << "\rDone.                 \n";
}

Vec3 Renderer::rayColor(const Ray& r, int depth, const Object& world) const {
    if(depth <= 0) return Vec3(0, 0, 0);

    Hit hitRecord;
    // 0.001 is the shadow-acne epsilon
    if(world.hit(r, Interval(0.001, infinity), hitRecord)){
        Ray scatteredRay;
        Vec3 attenuation;
        if(hitRecord.material->scatter(r, hitRecord, attenuation, scatteredRay)){
            return attenuation * rayColor(scatteredRay, depth - 1, world);
        }
        return Vec3(0, 0, 0);
    }

    // TODO: Replace with proper environment lighting once lighting is added
    Vec3 unitDirection = normalise(r.direction());
    double t = 0.5 * (unitDirection.y() + 1.0);
    return lerp(t, Vec3(0.5, 0.5, 0.5), Vec3(1.0, 1.0, 1.0));
}