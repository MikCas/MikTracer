#include "ImageBuffer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "third_party/stb_image_write.h"

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

