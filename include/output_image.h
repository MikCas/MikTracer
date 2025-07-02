#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class OutputImage {
public:
    OutputImage() = default;
    OutputImage(const std::string& outputName);
    ~OutputImage() = default;

    OutputImage(const OutputImage&) = delete;
    OutputImage& operator=(const OutputImage&) = delete;

    OutputImage(OutputImage&&) = default;
    OutputImage& operator=(OutputImage&&) = default;

    /**
     * Saves the rendered image to a PPM file.
     * @param width Width of the image in pixels.
     * @param height Height of the image in pixels.
     * @param frameBuffer Vector containing pixel data in RGB format.
     */
    void saveImage(int width, int height, const std::vector<float>& frameBuffer);

private: 
    std::string outputFileName_;  // Name of the output file
    std::ofstream outputFile_;  // Output file stream for writing the image
};