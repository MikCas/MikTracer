#include "../include/Camera.h"

Camera::Camera(double aspectRatio, int imageWidth, double focalLength, double viewportHeight) 
    : m_aspectRatio(aspectRatio), 
      m_imageWidth(imageWidth), 
      m_focalLength(focalLength),
      m_viewportHeight(viewportHeight) 
{
    m_imageHeight = static_cast<int>(m_imageWidth / m_aspectRatio);
    m_imageHeight = std::max(m_imageHeight, 1); // Ensure image height is at least 1

    // Set up camera and viewport
    m_origin = Vec3(0, 0, 0);
    m_viewportWidth = m_viewportHeight * (static_cast<double>(m_imageWidth) / m_imageHeight);
    m_viewportU = Vec3(m_viewportWidth, 0, 0);
    m_viewportV = Vec3(0, -m_viewportHeight, 0);

    // Calculate pixel offsets and the top-left corner of the viewport
    m_pixelOffsetU = m_viewportU / m_imageWidth;
    m_pixelOffsetV = m_viewportV / m_imageHeight;
    m_viewportTopLeft = m_origin - Vec3(0, 0, m_focalLength) - (m_viewportU / 2) - (m_viewportV / 2);
    m_pixel00 = m_viewportTopLeft + 0.5*(m_pixelOffsetU + m_pixelOffsetV);
}

void Camera::render(std::ofstream& outFile, const Object& world) {

    outFile << "P3\n" << m_imageWidth << " " << m_imageHeight << "\n255\n";

    for(int j = 0; j < m_imageHeight; j++){
        std::clog << "\rScanlines remaining: " << (m_imageHeight - j) << '\n' << std::flush;
        for(int i = 0; i < m_imageWidth; i++){
            
            // Calculate pixel center and ray direction
            Vec3 pixelCenter = m_pixel00 + (i * m_pixelOffsetU) + (j * m_pixelOffsetV);
            Vec3 rayDirection = pixelCenter - m_origin;
            Ray r(m_origin, rayDirection);

            // Calculate pixel color
            Vec3 color = rayColor(r, world);

            // Write pixel color to file
            writeColor(outFile, color);
        }
    }
    std::clog << "\rDone.                 \n";
}

Vec3 Camera::rayColor(const Ray& r, const Object& world) const {
        Hit hitRecord;
        if(world.hit(r, 0, infinity, hitRecord)){
            return 0.5 * (hitRecord.normal + Vec3(1, 1, 1));
        }
        Vec3 unitDirection = normalise(r.direction());
        double t = 0.5 * (unitDirection.y() + 1.0);

        return lerp(t, Vec3(1.0, 1.0, 1.0), Vec3(0.2, 0.3, 1.0));
}

void Camera::writeColor(std::ostream &out, const Vec3& color) const {
    double r = color.x();
    double g = color.y();
    double b = color.z();

    int rbyte = static_cast<int>(255.999 * r);
    int gbyte = static_cast<int>(255.999 * g);
    int bbyte = static_cast<int>(255.999 * b);

    out << rbyte << " " << gbyte << " " << bbyte << "\n";
}

