#pragma once

#include "Utility.h"
#include "Vec3.h"
#include "Ray.h"
#include "Object.h"
#include "Material.h"
#include "ImageBuffer.h"

class Camera {
private:   
    // Camera Settings
    Vec3 m_lookFrom;               // Camera origin - Point camera is looking from
    Vec3 m_lookAt;                 // Point camera is looking at
    Vec3 m_viewUp;                 // Up vector for the camera
    Vec3 m_basisU, m_basisV, m_basisW;   // Orthonormal basis vectors for the camera
    double m_verticalFOV;          // Vertical field of view in degrees
    double m_focusDistance;        // Distance from camera center (lookFrom) to a plane of perfect focus 
    double m_defocusAngle;         // Variation angle of rays through each pixel
    Vec3 m_defocusDiskU;           // Defocus disk horizontal radius
    Vec3 m_defocusDiskV;           // Defocus disk vertical radius
    
    // Viewport and image dimensions 
    double m_aspectRatio;          // Aspect ratio of the rendered image
    int m_imageWidth;              // Rendered image width
    int m_imageHeight;             // Rendered image height
    
    double m_viewportHeight;       // Height of the viewport
    double m_viewportWidth;        // Width of the viewport
    Vec3 m_viewportU;              // Vector across the horizontal viewport edge
    Vec3 m_viewportV;              // Vector across the vertical viewport edge
    Vec3 m_viewportTopLeft;        // Location of the top left corner of the viewport
    Vec3 m_pixel00;                // Location of the upper left pixel in the viewport
    Vec3 m_pixelOffsetU;           // Offset from one pixel to the next in the horizontal direction
    Vec3 m_pixelOffsetV;           // Offset from one pixel to the next in the vertical direction

    // Render Settings
    int m_samplesPerPixel;         // Number of samples per pixel - for anti-aliasing
    double m_pixelSampleScale;     // Scale factor for pixel samples
    int m_maxDepth;                // Maximum number of bounces for a ray

    // Camera functions
    Ray getRay(int i, int j) const;
    Vec3 sampleSquare() const;
    Vec3 defocusDiskSample() const;

    // Gamma correction
    double gammaCorrect(double linearValue) const;

    // Ray tracing
    Vec3 rayColor(const Ray& r, int depth, const Object& world, const Vec3& color1, const Vec3& color2) const;
    void writeColor(std::ostream &out, const Vec3& pixel) const;

public:

    Camera(Vec3 lookFrom, Vec3 lookAt, double aspectRatio, int imageWidth, int samplesPerPixel, int maxDepth, double verticalFOV, double focusDistance, double defocusAngle);
    void render(ImageBuffer& image, const Object& world, const Vec3& color1, const Vec3& color2); 
};