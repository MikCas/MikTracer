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
    int index = (j * m_width + i) * 3;
    m_pixels[index + 0] = toByte(linearColor.x());
    m_pixels[index + 1] = toByte(linearColor.y());
    m_pixels[index + 2] = toByte(linearColor.z());
}

void ImageBuffer::writePPM(const std::string& path) const{
    
    std::ofstream out(path);
    if (!out) {
        std::cerr << "ImageBuffer: could not open " << path << " for writing\n";
        return;
    }

    out << "P3\n" << m_width << " " << m_height << "\n255\n";

    for(int j = 0; j < m_height; j++){
        for(int i = 0; i < m_width; i++){
            int index = (j * m_width + i) * 3;
            out << static_cast<int>(m_pixels[index + 0]) << " "
                << static_cast<int>(m_pixels[index + 1]) << " "
                << static_cast<int>(m_pixels[index + 2]) << "\n";
        }
    }
}

void ImageBuffer::writePNG(const std::string& path) const { 
    int strideInBytes = m_width * 3;
    int success = stbi_write_png(
        path.c_str(),
        m_width, 
        m_height,
        3,
        m_pixels.data(),
        strideInBytes
    );

    if(!success) {
        std::cerr << "ImageBuffer: failed to write PNG to " << path << "\n";
    }
}

