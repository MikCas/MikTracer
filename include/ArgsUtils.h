#pragma once
#include <string>

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

/**
 * @brief Parses and validates command line arguments
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @param args Output structure to store parsed arguments
 * @return true if arguments are valid, false otherwise
 */
bool parseArguments(int argc, char* argv[], ProgramArgs& args);

