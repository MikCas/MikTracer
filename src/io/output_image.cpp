#include "output_image.h"

void outputRender(int width, int height, std::ofstream& outFile, const std::vector<float>& frameBuffer) {
    outFile << "P3\n" << width << " " << height << "\n255\n";

    std::ostringstream buffer;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int idx = (j * width + i) * 3;
            buffer << static_cast<int>(frameBuffer[idx] * 255) << ' '
                    << static_cast<int>(frameBuffer[idx + 1] * 255) << ' '
                    << static_cast<int>(frameBuffer[idx + 2] * 255) << "\n";
        }
    }
    outFile << buffer.str();
    outFile.close();
}