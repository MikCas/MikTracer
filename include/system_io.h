#pragma once

#include <cuda_runtime.h>
#include <fstream>
#include <iostream>
#include <string>

/**
 * @brief Structure to hold parsed command line arguments
 */
struct SystemArgs {
    int imageWidth;
    int imageHeight;
    std::string outputName;
    int blockSize;
};

struct SystemParams {
    int width;
    int height;
    int frameBufferSize;
    int frameBufferBytes;
    int blockSize;
    dim3 threads;  // CUDA thread configuration
    dim3 blocks;   // CUDA block configuration
};

class SystemIO {
public:
    SystemIO() = default;
    ~SystemIO() = default;

    SystemIO(const SystemIO&) = delete;
    SystemIO& operator=(const SystemIO&) = delete;
    
    SystemIO(SystemIO&&) = default;
    SystemIO& operator=(SystemIO&&) = default;

    bool parseArguments(int argc, char* argv[]);
    bool setupOutputFile();
    const SystemParams& setupSystemParameters();

    std::ofstream& getOutputFile() { return outFile_; }

    void showArguments();
    void showParameters();
    void showUsage();

private:
    SystemArgs args_;  // Holds parsed command line arguments
    SystemParams params_;  // Holds system parameters derived from arguments
    std::ofstream outFile_;  // Output file stream for writing the image
};















