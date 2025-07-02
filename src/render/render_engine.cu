#include "render_engine.h"

__global__ void render(int width, int height, float* frameBuffer) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if(i >= width || j >= height) return; // Check bounds

    int idx = (j * width + i) * 3;                         // 3 channels for RGB
    frameBuffer[idx] = static_cast<float>(i) / width;      // R
    frameBuffer[idx + 1] = static_cast<float>(j) / height; // G
    frameBuffer[idx + 2] = 0.0f;                           // B
}
