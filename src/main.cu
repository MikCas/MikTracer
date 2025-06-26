#include "utility-Args.h"
#include "utility-CUDA.h"
#include "utility-Display.h"

#include <vector>
// #include <cuda.h>    
// #include <time.h>

/**
 * @brief Render function to create a simple PPM image in an output file
 * @param width Image width in pixels
 * @param height Image height in pixels
 * @param outFile Output file stream to write the image data
 * @param frameBuffer Vector containing the pixel data in RGB format
 */
void outputRender(int width, int height, std::ofstream& outFile, const std::vector<float>& frameBuffer) {
    outFile << "P3\n" << width << " " << height << "\n255\n";
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int idx = (j * width + i) * 3;
            outFile << static_cast<int>(frameBuffer[idx] * 255) << ' '
                    << static_cast<int>(frameBuffer[idx + 1] * 255) << ' '
                    << static_cast<int>(frameBuffer[idx + 2] * 255) << "\n";
        }
    }
    outFile.close();
}

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
    checkCudaErrors(cudaMemcpy(frameBuffer_h.data(), frameBuffer_d, frameBufferBytes, cudaMemcpyDeviceToHost));
    checkCudaErrors(cudaFree(frameBuffer_d));

    // === OUTPUT ===
    outputRender(width, height, outFile, frameBuffer_h);
    std::cout << "Image saved to: " << outputFilePath << "\n";

    return 0;
}
