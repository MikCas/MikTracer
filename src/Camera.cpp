#include "Camera.h"

Camera::Camera(Vec3 lookFrom, Vec3 lookAt, double aspectRatio, int imageWidth, int samplesPerPixel, int maxDepth, double verticalFOV, double focusDistance, double defocusAngle) 
    : m_lookFrom(lookFrom),
      m_lookAt(lookAt),
      m_aspectRatio(aspectRatio), 
      m_imageWidth(imageWidth), 
      m_samplesPerPixel(samplesPerPixel), 
      m_maxDepth(maxDepth),
      m_verticalFOV(verticalFOV), 
      m_focusDistance(focusDistance),
      m_defocusAngle(defocusAngle)
{   
    // NOTE: The image height could be rounded down to an integer or set to a minimum value of 1 - so this differs from the actual imageWidth*aspectRatio value 
    m_imageHeight = static_cast<int>(m_imageWidth / m_aspectRatio);
    m_imageHeight = std::max(m_imageHeight, 1); // Ensure image height is at least 1
    double actualAspectRatio = static_cast<double>(m_imageWidth) / m_imageHeight;

    // Anti-aliasing
    m_pixelSampleScale  = 1.0 / m_samplesPerPixel;
    
    m_viewUp = Vec3(0, 1, 0);
    Vec3 viewDirection = m_lookFrom - m_lookAt;
    
    double theta = degreesToRadians(m_verticalFOV);
    double h = std::tan(theta / 2);
    // NOTE: Since the image height differs from the actual value, this is taken into account when calculating the viewport width
    m_viewportHeight = 2.0 * h * m_focusDistance;
    m_viewportWidth = m_viewportHeight * actualAspectRatio;

    // Calculate orthonormal basis vectors for the camera
    m_basisW = normalise(viewDirection);
    m_basisU = normalise(cross(m_viewUp, m_basisW));
    m_basisV = cross(m_basisW, m_basisU);

    m_viewportU = m_viewportWidth * m_basisU;
    m_viewportV = m_viewportHeight * -m_basisV;

    // Calculate pixel offsets and the top-left corner of the viewport
    m_pixelOffsetU = m_viewportU / m_imageWidth; // NOTE: The image width and height correspond to the number of pixels in the image
    m_pixelOffsetV = m_viewportV / m_imageHeight;
    m_viewportTopLeft = m_lookFrom - (m_focusDistance * m_basisW) - (m_viewportU / 2) - (m_viewportV / 2);
    m_pixel00 = m_viewportTopLeft + 0.5*(m_pixelOffsetU + m_pixelOffsetV);

    // Defocus disk basis vectors
    double defocusRadius = m_focusDistance * std::tan(degreesToRadians(m_defocusAngle / 2));
    m_defocusDiskU = m_basisU * defocusRadius;
    m_defocusDiskV = m_basisV * defocusRadius;
}

void Camera::render(ImageBuffer& image, const Object& world, const Vec3& color1, const Vec3& color2) {

    for(int j = 0; j < m_imageHeight; j++){
        std::clog << "\rScanlines remaining: " << (m_imageHeight - j) << '\n' << std::flush;
        for(int i = 0; i < m_imageWidth; i++){

            Vec3 color(0, 0, 0);

            // Anti-aliasing
            for(int sample = 0; sample < m_samplesPerPixel; sample++){
                Ray r = getRay(i, j);
                color += rayColor(r, m_maxDepth, world, color1, color2);
            }

            image.setPixel(i, j, color);
        }
    }
    std::clog << "\rDone.                 \n";
}

// Construct a camera ray originating from the origin and directed at randomly sampled point around the pixel location i, j.
Ray Camera::getRay(int i, int j) const {

    // Construct a camera ray originating from the defocus disk and directed at a randomly
    // sampled point around the pixel location i, j.

    Vec3 offset = sampleSquare();
    Vec3 sample = m_pixel00 
                + (i + offset.x()) * m_pixelOffsetU 
                + (j + offset.y()) * m_pixelOffsetV;


    Vec3 origin = (m_defocusAngle <= 0) ? m_lookFrom : defocusDiskSample();
    Vec3 direction = sample - origin;

    return Ray(origin, direction);
}

// Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
Vec3 Camera::sampleSquare() const {
    return  Vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
}

// Returns a random point on the defocus disk
Vec3 Camera::defocusDiskSample() const {
    Vec3 diskSample = randomVectorOnUnitDisk();
    return m_lookFrom + (m_defocusDiskU * diskSample[0]) + (m_defocusDiskV * diskSample[1]);
}

Vec3 Camera::rayColor(const Ray& r, int depth, const Object& world, const Vec3& color1, const Vec3& color2) const {

        if(depth <= 0){
            return Vec3(0, 0, 0);
        }

        Hit hitRecord;
        if(world.hit(r, Interval(0.001, infinity), hitRecord)){
            Ray scatteredRay;
            Vec3 attenuation;
            if(hitRecord.material->scatter(r, hitRecord, attenuation, scatteredRay)){
                return attenuation * rayColor(scatteredRay, depth - 1, world, color1, color2);
            }
            return Vec3(0, 0, 0);
        }

        Vec3 unitDirection = normalise(r.direction());
        double t = 0.5 * (unitDirection.y() + 1.0);
        return lerp(t, color1, color2);
}