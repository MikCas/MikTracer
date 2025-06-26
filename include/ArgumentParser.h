#pragma once
#include <iostream>
#include <fstream>

/**
 * @brief Structure to hold parsed command line arguments
 */
struct ProgramArgs {
    int imageWidth;
    int imageHeight;
    std::string outputFileName;
    int blockSize;
};

/**
 * @brief Constructs the full file path from base name
 * @param baseName Base filename without extension
 * @return Full path with renders/ directory and .ppm extension
 */
std::string constructFilePath(const std::string& baseName);

/**
 * @brief Prints usage information for the program
 * @param programName Name of the executable
 */
void printUsage(const char* programName);


bool parseArguments(int argc, char* argv[], ProgramArgs& args);
