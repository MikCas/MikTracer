#include "ImageBuffer.h"
#include "Interval.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "third_party/stb_image_write.h"

#include <cmath>
#include <fstream>
#include <iostream>

namespace {
    // Linear -> gamma2 -> [0, 255] byte
    uint8_t toByte(double linearValue) {

        // Gamma correction
        double gammaCorrected = (linearValue > 0) ? std::sqrt(linearValue) : 0.0;
        
        // Clamp to [0, 0.999] then scale to byte
        // Clamped at 0.999 to prevent integer overflow (256 * 0.999 = 255.744) since byte \in [0, 255]
        static const Interval intensity(0.000, 0.999);
        return static_cast<uint8_t>(256 * intensity.clamp(gammaCorrected));
    }
}

ImageBuffer::ImageBuffer(int width, int height)
    : m_width(width), m_height(height), m_pixels(width * height * 3, 0) { }

int ImageBuffer::width() const { return m_width; }
int ImageBuffer::height() const { return m_height; }

void ImageBuffer::setPixel(int i, int j, const Vec3& linearColor) {

}

void ImageBuffer::writePPM(const std::string& path) const{

}

void ImageBuffer::writePNG(const std::string& path) const { 

}

