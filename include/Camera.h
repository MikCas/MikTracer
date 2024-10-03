#pragma once

#include "Object.h"
#include "Ray.h"
#include "Utility.h"

class Camera {
private:   

    Vec3 m_origin;                 // Camera origin
    Vec3 m_pixel00;                // Location of the upper left pixel in the viewport
    Vec3 m_pixelOffsetU;           // Offset from one pixel to the next in the horizontal direction
    Vec3 m_pixelOffsetV;           // Offset from one pixel to the next in the vertical direction
    double m_focalLength = 1.0;    // Focal length of the camera

    double m_aspectRatio = 1.0;    // Aspect ratio of the rendered image
    int m_imageWidth = 100;        // Rendered image width
    int m_imageHeight;             // Rendered image height
    
    double m_viewportHeight = 2.0; // Height of the viewport
    double m_viewportWidth;        // Width of the viewport
    Vec3 m_viewportU;              // Vector across the horizontal viewport edge
    Vec3 m_viewportV;              // Vector across the vertical viewport edge
    Vec3 m_viewportTopLeft;        // Location of the top left corner of the viewport

    void initialise() {
        m_imageHeight = static_cast<int>(m_imageWidth / m_aspectRatio);
        m_imageHeight = std::max(m_imageHeight, 1);

        m_origin = Vec3(0, 0, 0);

        m_viewportWidth = m_viewportHeight * (static_cast<double>(m_imageWidth) / m_imageHeight);
        m_viewportU = Vec3(m_viewportWidth, 0, 0);
        m_viewportV = Vec3(0, -m_viewportHeight, 0);

        m_pixelOffsetU = m_viewportU / m_imageWidth;
        m_pixelOffsetV = m_viewportV / m_imageHeight;

        m_viewportTopLeft = m_origin - Vec3(0, 0, m_focalLength) - (m_viewportU / 2) - (m_viewportV / 2);
        m_pixel00 = m_viewportTopLeft + 0.5*(m_pixelOffsetU + m_pixelOffsetV);
    }

    Vec3 rayColor(const Ray& r, const Object& world) const {
        Hit hitRecord;
        if(world.hit(r, 0, infinity, hitRecord)){
            return 0.5 * (hitRecord.normal + Vec3(1, 1, 1));
        }
        Vec3 unitDirection = normalise(r.direction());
        double t = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
    }

public:

    Camera(double aspectRatio, int imageWidth) : m_aspectRatio(aspectRatio), m_imageWidth(imageWidth) {
        initialise();
    }

    void render(std::ofstream& outFile, const Object& world) {

        outFile << "P3\n" << m_imageWidth << " " << m_imageHeight << "\n255\n";

        for(int j = 0; j < m_imageHeight; j++){
            std::clog << "\rScanlines remaining: " << (m_imageHeight - j) << '\n' << std::flush;
            for(int i = 0; i < m_imageWidth; i++){
                
                Vec3 pixelCenter = m_pixel00 + (i * m_pixelOffsetU) + (j * m_pixelOffsetV);
                Vec3 rayDirection = pixelCenter - m_origin;
                Ray r(m_origin, rayDirection);

                Vec3 pixelColor = rayColor(r, world);

                writeColor(outFile, pixelColor);
            }
        }
        std::clog << "\rDone.                 \n";
    }
};