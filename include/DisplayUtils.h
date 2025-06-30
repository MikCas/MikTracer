#pragma once
#include <vector>
#include <fstream>
#include <sstream>

/**
 * @brief Render function to create a simple PPM image in an output file
 * @param width Image width in pixels
 * @param height Image height in pixels
 * @param outFile Output file stream to write the image data
 * @param frameBuffer Vector containing the pixel data in RGB format
 */
void outputRender(int width, int height, std::ofstream& outFile, const std::vector<float>& frameBuffer);