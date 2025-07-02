#include "system_io.h"
#include "output_image.h"
#include "gpu_utils.h"
#include "render_engine.h"

#include <iostream>
#include <fstream> 
#include <vector>
#include <time.h>


int main(int argc, char* argv[]) {

    // === SETUP ===
    SystemIO systemIO;
    if (!systemIO.parseArguments(argc, argv)) return 1;
    if (!systemIO.setupOutputFile()) return 1; 

    const SystemParams& params = systemIO.setupSystemParameters();

    // === H2D ===
    std::vector<float> frameBuffer_h(params.frameBufferSize, 0.0f);
    float* frameBuffer_d;
    checkCudaErrors(cudaMalloc((void**)&frameBuffer_d, params.frameBufferBytes));
    checkCudaErrors(cudaMemcpy(frameBuffer_d, frameBuffer_h.data(), params.frameBufferBytes, cudaMemcpyHostToDevice));

    // === RENDER ===
    clock_t start, end;
    start = clock();
    render<<<params.blocks, params.threads>>>(params.width, params.height, frameBuffer_d);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());
    end = clock();
    double elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    std::cout << "Kernel execution time: " << elapsed << " seconds\n";

    // === D2H ===
    start = clock();
    checkCudaErrors(cudaMemcpy(frameBuffer_h.data(), frameBuffer_d, params.frameBufferBytes, cudaMemcpyDeviceToHost));
    checkCudaErrors(cudaFree(frameBuffer_d));
    end = clock();  
    elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    std::cout << "Memory transfer time: " << elapsed << " seconds\n";

    // === OUTPUT ===
    start = clock();
    outputRender(params.width, params.height, systemIO.getOutputFile(), frameBuffer_h);
    end = clock();
    elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    std::cout << "Output time: " << elapsed << " seconds\n";

    return 0;
}
