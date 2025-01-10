#include <cuda_runtime.h>
#include <stdio.h>
#include "conv.h"

__global__ void apply_kernel_cuda(double *d_grayscale, double *d_output, int width, int height, double *d_kernel) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < width && y < height) {
        double sum = 0.0;
        for (int ky = 0; ky < KERNEL_SIZE; ky++) {
            for (int kx = 0; kx < KERNEL_SIZE; kx++) {
                int px = x + kx - KERNEL_SIZE / 2;
                int py = y + ky - KERNEL_SIZE / 2;
                if (px >= 0 && px < width && py >= 0 && py < height) {
                    sum += d_grayscale[py * width + px] * d_kernel[ky * KERNEL_SIZE + kx];
                }
            }
        }
        d_output[y * width + x] = sum;
    }
}

extern "C" void apply_kernel_cuda_wrapper(double *grayscale, double *output, int width, int height, double **kernel, int block_size) {
    double *d_grayscale, *d_output, *d_kernel;
    size_t size = width * height * sizeof(double);
    size_t kernel_size = KERNEL_SIZE * KERNEL_SIZE * sizeof(double);

    // Allocate device memory
    cudaMalloc((void **)&d_grayscale, size);
    cudaMalloc((void **)&d_output, size);
    cudaMalloc((void **)&d_kernel, kernel_size);

    // Copy data from host to device
    cudaMemcpy(d_grayscale, grayscale, size, cudaMemcpyHostToDevice);

    // Flatten the kernel for copying to device
    double *flattened_kernel = (double *)malloc(kernel_size);
    for (int i = 0; i < KERNEL_SIZE; i++) {
        for (int j = 0; j < KERNEL_SIZE; j++) {
            flattened_kernel[i * KERNEL_SIZE + j] = kernel[i][j];
        }
    }
    cudaMemcpy(d_kernel, flattened_kernel, kernel_size, cudaMemcpyHostToDevice);
    free(flattened_kernel);

    // Define block and grid sizes
    dim3 threadsPerBlock(block_size, block_size);
    dim3 numBlocks((width + threadsPerBlock.x - 1) / threadsPerBlock.x, (height + threadsPerBlock.y - 1) / threadsPerBlock.y);

    // Launch the kernel
    apply_kernel_cuda<<<numBlocks, threadsPerBlock>>>(d_grayscale, d_output, width, height, d_kernel);

    // Check for kernel launch errors
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to launch kernel: %s\n", cudaGetErrorString(err));
        cudaFree(d_grayscale);
        cudaFree(d_output);
        cudaFree(d_kernel);
        return;
    }

    // Copy data from device to host
    cudaMemcpy(output, d_output, size, cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_grayscale);
    cudaFree(d_output);
    cudaFree(d_kernel);

    // Reset the device to clear the cache
    cudaDeviceReset();
}