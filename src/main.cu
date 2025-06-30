#include "ArgsUtils.h"
#include "DisplayUtils.h"
#include "CUDAUtils.h"

#include <iostream>
#include <fstream> 
#include <vector>
#include <time.h>

__global__ void render(int width, int height, float* frameBuffer) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if(i >= width || j >= height) return; // Check bounds

    int idx = (j * width + i) * 3;                         // 3 channels for RGB
    frameBuffer[idx] = static_cast<float>(i) / width;      // R
    frameBuffer[idx + 1] = static_cast<float>(j) / height; // G
    frameBuffer[idx + 2] = 0.0f;                           // B
}

int main(int argc, char* argv[]) {

    // === PARSE ARGUMENTS ===
    ProgramArgs args;
    if (!parseArguments(argc, argv, args)) {
        return 1; // Exit if arguments are invalid
    }

    const std::string outputFilePath = constructFilePath(args.outputFileName);
    std::ofstream outFile(outputFilePath);
    if (!outFile) {
        std::cerr << "Error: Cannot open file " << args.outputFileName << " for writing\n";
        return 1;
    }

    // === SETUP ===
    const int width = args.imageWidth;
    const int height = args.imageHeight;
    const int frameBufferSize = width * height * 3; // RGB
    const int frameBufferBytes = frameBufferSize * sizeof(float);

    const int blockSize = std::min(args.blockSize, 32); // Cap at 32 for 2D grids, since 1024 (=32x32) is the max threads per block
    const dim3 threads(blockSize, blockSize);
    const dim3 blocks((width - 1)/blockSize + 1, (height - 1)/blockSize + 1);

    std::cout << "Rendering " << width << "x" << height 
                << " with blocks(" << blocks.x << "," << blocks.y 
                << ") threads(" << threads.x << "," << threads.y << ")\n";

    // === MEMORY ALLOCATION ===
    std::vector<float> frameBuffer_h(frameBufferSize, 0.0f);
    float* frameBuffer_d;
    checkCudaErrors(cudaMalloc((void**)&frameBuffer_d, frameBufferBytes));
    checkCudaErrors(cudaMemcpy(frameBuffer_d, frameBuffer_h.data(), frameBufferBytes, cudaMemcpyHostToDevice));

    // === RENDER ===
    clock_t start, end;
    start = clock();

    render<<<blocks, threads>>>(width, height, frameBuffer_d);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());

    end = clock();
    double elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    std::cout << "Kernel execution time: " << elapsed << " seconds\n";

    // === TRANSFER BACK ===
    start = clock();

    checkCudaErrors(cudaMemcpy(frameBuffer_h.data(), frameBuffer_d, frameBufferBytes, cudaMemcpyDeviceToHost));
    checkCudaErrors(cudaFree(frameBuffer_d));

    end = clock();  
    elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    std::cout << "Memory transfer time: " << elapsed << " seconds\n";

    // === OUTPUT ===
    start = clock();

    outputRender(width, height, outFile, frameBuffer_h);

    end = clock();
    elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    std::cout << "Output time: " << elapsed << " seconds\n";

    std::cout << "Image saved to: " << outputFilePath << "\n";
    return 0;
}
