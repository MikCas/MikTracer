#pragma once 

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

// // Simple memory management helpers
// template<typename T>
// inline T* cuda_alloc(size_t count) {
//     T* ptr;
//     CHECK_CUDA(cudaMalloc(&ptr, count * sizeof(T)));
//     return ptr;
// }

// template<typename T>
// inline void cuda_copy_h2d(T* d_ptr, const T* h_ptr, size_t count) {
//     CHECK_CUDA(cudaMemcpy(d_ptr, h_ptr, count * sizeof(T), cudaMemcpyHostToDevice));
// }

// template<typename T>
// inline void cuda_copy_d2h(T* h_ptr, const T* d_ptr, size_t count) {
//     CHECK_CUDA(cudaMemcpy(h_ptr, d_ptr, count * sizeof(T), cudaMemcpyDeviceToHost));
// }

// // Safe kernel launch configuration
// inline dim3 safe_grid_size(int width, int height, int block_size = 16) {
//     return dim3((width + block_size - 1) / block_size, 
//                (height + block_size - 1) / block_size);
// }

// // Device info
// inline void print_gpu_info() {
//     cudaDeviceProp prop;
//     CHECK_CUDA(cudaGetDeviceProperties(&prop, 0));
//     std::cout << "GPU: " << prop.name 
//               << " | Max threads/block: " << prop.maxThreadsPerBlock << "\n";
// }