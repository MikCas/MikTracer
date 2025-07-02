#include "render_config.h"

bool RenderConfig::parseArguments(int argc, char* argv[]) {

    // === CHECK ARGUMENT COUNT ===
    if (argc != 5) {
        std::cerr << "Error: Expected 5 arguments, got " << (argc - 1) << "\n\n";
        showUsage();
        return false;
    }

    // === PARSE ARGUMENTS === 
    args_.imageWidth = std::atoi(argv[1]);
    args_.imageHeight = std::atoi(argv[2]);
    args_.outputName = argv[3];
    args_.blockSize = std::atoi(argv[4]);

    // === VALIDATE ARGUMENTS ===
    if (args_.imageWidth <= 0) {
        std::cerr << "Error: Width must be a positive integer, got " << argv[1] << "\n";
        return false;
    }

    if (args_.imageHeight <= 0) {
        std::cerr << "Error: Height must be a positive integer, got " << argv[2] << "\n";
        return false;
    }

    if (args_.blockSize <= 0) {
        std::cerr << "Error: Block size must be a positive integer, got " << argv[4] << "\n";
        return false;
    }

    setupParameters();
    return true;
}

void RenderConfig::setupParameters() {

    params_.width = args_.imageWidth;
    params_.height = args_.imageHeight;
    params_.frameBufferSize = params_.width * params_.height * 3; // RGB
    params_.frameBufferBytes = params_.frameBufferSize * sizeof(float);

    // Set CUDA thread and block configuration
    params_.blockSize = std::min(args_.blockSize, 32); // Cap at 32 for 2D grids, since 1024 (=32x32) is the max threads per block
    params_.threads = dim3(params_.blockSize, params_.blockSize);
    params_.blocks = dim3((params_.width - 1) / params_.blockSize + 1,
                          (params_.height - 1) / params_.blockSize + 1);

}

void RenderConfig::showArguments() {
    std::cout << "Image Width: " << args_.imageWidth << "\n";
    std::cout << "Image Height: " << args_.imageHeight << "\n";
    std::cout << "Output File Name: " << args_.outputName << "\n";
    std::cout << "Block Size: " << args_.blockSize << "\n";
}

void RenderConfig::showParameters() {
    std::cout << "System Parameters:\n";
    std::cout << "  Width: " << params_.width << "\n";
    std::cout << "  Height: " << params_.height << "\n";
    std::cout << "  Frame Buffer Size: " << params_.frameBufferSize << "\n";
    std::cout << "  Frame Buffer Bytes: " << params_.frameBufferBytes << "\n";
    std::cout << "  Block Size: " << params_.blockSize << "\n";
    std::cout << "  Threads: (" << params_.threads.x << ", " << params_.threads.y << ")\n";
    std::cout << "  Blocks: (" << params_.blocks.x << ", " << params_.blocks.y << ")\n";
}

void RenderConfig::showUsage() {
    std::cout << "  width           - Image width in pixels (e.g., 256)\n";
    std::cout << "  height          - Image height in pixels (e.g., 256)\n";
    std::cout << "  output_filename - Name of the PPM file to create (e.g., image.ppm)\n";
    std::cout << "  blockSize       - Size of the blocks to render (default is 8)\n";
}

