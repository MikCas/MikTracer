#include "ArgsUtils.h"
#include <iostream>
#include <fstream>


std::string constructFilePath(const std::string& baseName) {
    return "renders/" + baseName + ".ppm";
}

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <width> <height> <output_filename>\n";
    std::cout << "  width           - Image width in pixels (e.g., 256)\n";
    std::cout << "  height          - Image height in pixels (e.g., 256)\n";
    std::cout << "  output_filename - Name of the PPM file to create (e.g., image.ppm)\n";
    std::cout << "  blockSize       - Size of the blocks to render (default is 8)\n";
    std::cout << "\nExample: " << programName << " 400 300 my_image.ppm, 8\n";
}

bool parseArguments(int argc, char* argv[], ProgramArgs& args) {

    // === CHECK ARGUMENT COUNT ===
    if (argc != 5) {
        std::cerr << "Error: Expected 5 arguments, got " << (argc - 1) << "\n\n";
        printUsage(argv[0]);
        return false;
    }

    // === PARSE ARGUMENTS === 
    args.imageWidth = std::atoi(argv[1]);
    args.imageHeight = std::atoi(argv[2]);
    args.outputFileName = argv[3];
    args.blockSize = std::atoi(argv[4]);

    // === VALIDATE ARGUMENTS ===
    if (args.imageWidth <= 0) {
        std::cerr << "Error: Width must be a positive integer, got " << argv[1] << "\n";
        return false;
    }

    if (args.imageHeight <= 0) {
        std::cerr << "Error: Height must be a positive integer, got " << argv[2] << "\n";
        return false;
    }

    if (args.blockSize <= 0) {
        std::cerr << "Error: Block size must be a positive integer, got " << argv[4] << "\n";
        return false;
    }

    return true;
}
