#include "../include/Camera.h"

Camera::Camera(double aspectRatio, int imageWidth, double focalLength, double viewportHeight, int samplesPerPixel) 
    : m_aspectRatio(aspectRatio), 
      m_imageWidth(imageWidth), 
      m_focalLength(focalLength),
      m_viewportHeight(viewportHeight), 
      m_samplesPerPixel(samplesPerPixel)
{   
    // NOTE: The image height could be rounded down to an integer or set to a minimum value of 1 - so this differs from the actual imageWidth*aspectRatio value 
    m_imageHeight = static_cast<int>(m_imageWidth / m_aspectRatio);
    m_imageHeight = std::max(m_imageHeight, 1); // Ensure image height is at least 1
    double actualAspectRatio = static_cast<double>(m_imageWidth) / m_imageHeight;

    // Anti-aliasing
    m_pixelSampleScale  = 1.0 / m_samplesPerPixel;

    // Set up camera and viewport
    m_origin = Vec3(0, 0, 0);
    // NOTE: Since the image height differs from the actual value, this is taken into account when calculating the viewport width
    m_viewportWidth = m_viewportHeight * actualAspectRatio;
    m_viewportU = Vec3(m_viewportWidth, 0, 0);
    m_viewportV = Vec3(0, -m_viewportHeight, 0);

    // Calculate pixel offsets and the top-left corner of the viewport
    m_pixelOffsetU = m_viewportU / m_imageWidth; // NOTE: The image width and height correspond to the number of pixels in the image
    m_pixelOffsetV = m_viewportV / m_imageHeight;
    m_viewportTopLeft = m_origin - Vec3(0, 0, m_focalLength) - (m_viewportU / 2) - (m_viewportV / 2);
    m_pixel00 = m_viewportTopLeft + 0.5*(m_pixelOffsetU + m_pixelOffsetV);
}

void Camera::render(std::ofstream& outFile, const Object& world) {

    outFile << "P3\n" << m_imageWidth << " " << m_imageHeight << "\n255\n";

    for(int j = 0; j < m_imageHeight; j++){
        std::clog << "\rScanlines remaining: " << (m_imageHeight - j) << '\n' << std::flush;
        for(int i = 0; i < m_imageWidth; i++){

            Vec3 color(0, 0, 0);

            for(int sample = 0; sample < m_samplesPerPixel; sample++){
                Ray r = getRay(i, j);
                color += rayColor(r, world);
            }
        
            // Write pixel color to file
            writeColor(outFile, m_pixelSampleScale * color);
        }
    }
    std::clog << "\rDone.                 \n";
}

// Construct a camera ray originating from the origin and directed at randomly sampled point around the pixel location i, j.
Ray Camera::getRay(int i, int j) const {

    Vec3 offset = sampleSquare();
    Vec3 sample = m_pixel00 
                + (i + offset.x()) * m_pixelOffsetU 
                + (j + offset.y()) * m_pixelOffsetV;

    Vec3 rayOrigin = m_origin;
    Vec3 rayDirection = sample - m_origin;

    return Ray(rayOrigin, rayDirection);
}

// Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
Vec3 Camera::sampleSquare() const {
    return  Vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
}

Vec3 Camera::rayColor(const Ray& r, const Object& world) const {
        Hit hitRecord;
        if(world.hit(r, Interval(0, infinity), hitRecord)){
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

    static const Interval intensity(0.000, 0.999);
    int rbyte = static_cast<int>(256 * intensity.clamp(r));
    int gbyte = static_cast<int>(256 * intensity.clamp(g));
    int bbyte = static_cast<int>(256 * intensity.clamp(b));

    out << rbyte << " " << gbyte << " " << bbyte << "\n";
}

