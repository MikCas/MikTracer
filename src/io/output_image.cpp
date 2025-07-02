#include "output_image.h"

OutputImage::OutputImage(const std::string& outputName) {
    outputFileName_ = "renders/" + outputName + ".ppm"; 
    outputFile_.open(outputFileName_);
    if (!outputFile_) {
        throw std::runtime_error("Could not create output file " + outputFileName_);
    }
}

void OutputImage::saveImage(int width, int height, const std::vector<float>& frameBuffer) {
    outputFile_ << "P3\n" << width << " " << height << "\n255\n";

    std::ostringstream buffer;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int idx = (j * width + i) * 3;
            buffer << static_cast<int>(frameBuffer[idx] * 255) << ' '
                    << static_cast<int>(frameBuffer[idx + 1] * 255) << ' '
                    << static_cast<int>(frameBuffer[idx + 2] * 255) << "\n";
        }
    }
    outputFile_ << buffer.str();
    outputFile_.close();
}
