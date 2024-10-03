#pragma once

#include "Utility.h"
#include "Object.h"

class Camera {
private:   

    // Camera Settings
    Vec3 m_origin;                 // Camera origin
    double m_focalLength;          // Focal length of the camera

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

    // Anti-aliasing functions
    Ray getRay(int i, int j) const;
    Vec3 sampleSquare() const;

    Vec3 rayColor(const Ray& r, const Object& world) const;
    void writeColor(std::ostream &out, const Vec3& pixel) const;

public:

    Camera(double aspectRatio, int imageWidth, double focalLength = 1.0, double viewportHeight = 2.0, int samplesPerPixel = 100);
    void render(std::ofstream& outFile, const Object& world); 
};