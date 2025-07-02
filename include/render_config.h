#pragma once

#include <cuda_runtime.h>
#include <iostream>


/**
 * @brief Structure to hold parsed command line arguments
 */
struct ConfigArgs {
    int imageWidth;
    int imageHeight;
    std::string outputName;
    int blockSize;
};

struct ConfigParams {
    int width;
    int height;
    int frameBufferSize;
    int frameBufferBytes;
    int blockSize;
    dim3 threads;  // CUDA thread configuration
    dim3 blocks;   // CUDA block configuration
};

class RenderConfig {
public:
    RenderConfig() = default;
    ~RenderConfig() = default;

    RenderConfig(const RenderConfig&) = delete;
    RenderConfig& operator=(const RenderConfig&) = delete;

    RenderConfig(RenderConfig&&) = default;
    RenderConfig& operator=(RenderConfig&&) = default;

    const ConfigParams& getParams() const { return params_; }
    const std::string getOutputName() const { return args_.outputName; }

    bool parseArguments(int argc, char* argv[]);
    void setupParameters();

    void showArguments();
    void showParameters();
    void showUsage();

private:
    ConfigArgs args_;  // Holds parsed command line arguments
    ConfigParams params_;  // Holds system parameters derived from arguments
};















