#pragma once

#include "Utility.h"
#include "Vec3.h"
#include "Ray.h"
#include "Object.h"
#include "Material.h"
#include "ImageBuffer.h"

struct CameraSettings {
    Vec3   lookFrom        = Vec3(0, 0, 0);   // Camera origin - Point camera is looking from
    Vec3   lookAt          = Vec3(0, 0, -1);  // Point camera is looking at
    Vec3   viewUp          = Vec3(0, 1, 0);   // Up vector for the camera
    double aspectRatio     = 16.0 / 9.0;
    int    imageWidth      = 400;
    int    samplesPerPixel = 100;             // Number of samples per pixel - for anti-aliasing
    int    maxDepth        = 50;              // Maximum number of bounces for a ray  
    double verticalFOV     = 90.0;            // Vertical field of view in degrees
    double focusDistance   = 10.0;            // Distance from camera center (lookFrom) to a plane of perfect focus 
    double defocusAngle    = 0.0;             // Variation angle of rays through each pixel
};

class Camera {
private:   

    CameraSettings m_settings;

    // Derived variables, calculated once in constructor         
    int m_imageHeight;            
    double m_viewportHeight;       
    double m_viewportWidth;       

    Vec3 m_basisU, m_basisV, m_basisW;  // Orthonormal basis vectors for the camera
    Vec3 m_viewportU;                   // Vector across the horizontal viewport edge
    Vec3 m_viewportV;                   // Vector across the vertical viewport edge
    Vec3 m_viewportTopLeft;             // Location of the top left corner of the viewport
    Vec3 m_pixel00;                     // Location of the upper left pixel in the viewport
    Vec3 m_pixelOffsetU;                // Offset from one pixel to the next in the horizontal direction
    Vec3 m_pixelOffsetV;                // Offset from one pixel to the next in the vertical direction

    // Render Settings
    double m_pixelSampleScale;     // Scale factor for pixel samples
    Vec3 m_defocusDiskU;           // Defocus disk horizontal radius
    Vec3 m_defocusDiskV;           // Defocus disk vertical radius    

    // Camera functions
    Ray getRay(int i, int j) const;
    Vec3 sampleSquare() const;
    Vec3 defocusDiskSample() const;

    // Ray tracing functions
    Vec3 rayColor(const Ray& r, int depth, const Object& world) const;

public:

    Camera(const CameraSettings& settings);

    int imageWidth() const {return m_settings.imageWidth; }
    int imageHeight() const {return m_imageHeight; }

    void render(ImageBuffer& image, const Object& world) const; 
};