#include "render_config.h"
#include "output_image.h"
#include "tuple3.h"

#include <cuda_runtime.h>
#include <iostream>
#include <fstream> 
#include <vector>
#include <time.h>

#define checkCudaErrors(val) check_cuda( (val), #val, __FILE__, __LINE__ )
void check_cuda(cudaError_t result, char const *const func, const char *const file, int const line) {
    if (result) {
        std::cerr << "CUDA error = " << static_cast<unsigned int>(result) << " at " <<
        file << ":" << line << " '" << func << "' \n";
        // Make sure we call CUDA Device Reset before exiting
        cudaDeviceReset();
        exit(99);
    }
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

    // === SETUP ===
    RenderConfig config;
    if (!config.parseArguments(argc, argv)) return 1;
    const ConfigParams& params = config.getParams();

    OutputImage outputImage(config.getOutputName());

    // === H2D ===
    std::vector<float> frameBuffer_h(params.frameBufferSize, 0.0f);
    float* frameBuffer_d;
    checkCudaErrors(cudaMalloc((void**)&frameBuffer_d, params.frameBufferBytes));
    checkCudaErrors(cudaMemcpy(frameBuffer_d, frameBuffer_h.data(), params.frameBufferBytes, cudaMemcpyHostToDevice));

    // === RENDER ===
    render<<<params.blocks, params.threads>>>(params.width, params.height, frameBuffer_d);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());

    // === D2H ===
    checkCudaErrors(cudaMemcpy(frameBuffer_h.data(), frameBuffer_d, params.frameBufferBytes, cudaMemcpyDeviceToHost));
    checkCudaErrors(cudaFree(frameBuffer_d));

    // === OUTPUT ===
    outputImage.saveImage(params.width, params.height, frameBuffer_h);

    return 0;
}
