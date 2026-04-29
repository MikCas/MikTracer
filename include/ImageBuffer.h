#pragma once

#include "Vec3.h"

#include <cstdint>
#include <string>
#include <vector>

class ImageBuffer {
private:
    int m_width;
    int m_height;
    std::vector<uint8_t> m_pixels; //width * height * 3 (RGB8)

public:
    ImageBuffer(int width, int height);

    int width() const;
    int height() const;

    // Writes a single pixel, linearColor is in [0, 1] linear space
    // Applies Gamma correction and clamping to 8-bit
    void setPixel(int i, int j, const Vec3& linearColor);
    
    // File I/O
    void writePPM(const std::string& path) const;
    void writePNG(const std::string& path) const;
};