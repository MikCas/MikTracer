#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

/**
* @file main.cpp
*/

/**
 * @brief Structure to hold parsed command line arguments
 */
struct ProgramArgs {
    int width;
    int height;
    std::string outputFileName;
};

/**
 * @brief Prints usage information for the program
 * @param programName Name of the executable
 */
void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <width> <height> <output_filename>\n";
    std::cout << "  width           - Image width in pixels (e.g., 256)\n";
    std::cout << "  height          - Image height in pixels (e.g., 256)\n";
    std::cout << "  output_filename - Name of the PPM file to create (e.g., image.ppm)\n";
    std::cout << "\nExample: " << programName << " 400 300 my_image.ppm\n";
}

/**
 * @brief Parses and validates command line arguments
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @param args Output structure to store parsed arguments
 * @return true if arguments are valid, false otherwise
 */
bool parseArguments(int argc, char* argv[], ProgramArgs& args) {

    // === CHECK ARGUMENT COUNT ===
    if (argc != 4) {
        std::cerr << "Error: Expected 3 arguments, got " << (argc - 1) << "\n\n";
        printUsage(argv[0]);
        return false;
    }

    // === PARSE ARGUMENTS === 
    args.width = std::atoi(argv[1]);
    args.height = std::atoi(argv[2]);
    args.outputFileName = argv[3];

    // === VALIDATE ARGUMENTS ===
    if (args.width <= 0) {
        std::cerr << "Error: Width must be a positive integer, got " << argv[1] << "\n";
        return false;
    }

    if (args.height <= 0) {
        std::cerr << "Error: Height must be a positive integer, got " << argv[2] << "\n";
        return false;
    }

    return true;
}

/**
 * @brief Constructs the full file path from base name
 * @param baseName Base filename without extension
 * @return Full path with renders/ directory and .ppm extension
 */
std::string constructFilePath(const std::string& baseName) {
    return "renders/" + baseName + ".ppm";
}

/**
 * @brief Render function to create a simple PPM image in an output file
 * @param width Image width in pixels
 * @param height Image height in pixels
 * @param outFile Output file stream to write the image data
 */
void simpleRender(int width, int height, std::ofstream& outFile) {
    outFile << "P3\n" << width << " " << height << "\n255\n";
    for (int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            outFile << i << ' ' << j << ' ' << 0 << "\n";
        }
    }
    outFile.close();
}

int main(int argc, char* argv[]) {

    // === PARSE COMMAND LINE ARGUMENTS ===
    ProgramArgs args;
    if (!parseArguments(argc, argv, args)) {
        return 1; // Exit if arguments are invalid
    }

    // === OPEN OUTPUT FILE ===
    std::string outputFilePath = constructFilePath(args.outputFileName);
    std::ofstream outFile(outputFilePath);
    if (!outFile) {
        std::cerr << "Error: Cannot open file '" << args.outputFileName << "' for writing\n";
        return 1;
    }

    // === RENDER ===
    std::cout << "Creating " << args.width << "x" << args.height  << " image: " << outputFilePath << "\n";
    simpleRender(args.width, args.height, outFile);
    std::cout << "Successfully created image: " << outputFilePath << "\n";

    return 0;
}