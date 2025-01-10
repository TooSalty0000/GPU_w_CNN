// extract pixel values from the image
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <png.h>
#include <time.h>
#include <string.h>
#include "conv.h"
#include "image.h"

void lap_gaus_kernel(double **kernel, int kernel_size, double sigma) {
    double sum = 0.0;
    int half_size = kernel_size / 2;

    for (int i = 0; i < kernel_size; i++) {
        for (int j = 0; j < kernel_size; j++) {
            double x = i - half_size;
            double y = j - half_size;
            double value = (1 / (2 * 3.14159 * pow(sigma, 4))) * 
                           (1 - ((x * x + y * y) / (2 * sigma * sigma))) * 
                           exp(-(x * x + y * y) / (2 * sigma * sigma));
            kernel[i][j] = value;
            sum += fabs(value);
        }
    }

    // Normalize the kernel
    for (int i = 0; i < kernel_size; i++) {
        for (int j = 0; j < kernel_size; j++) {
            kernel[i][j] /= sum;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    png_bytep *row_pointers = NULL;
    unsigned char *image = NULL;
    double *grayscale;
    double *output;
    int width, height;

    printf("Reading image %s\n", filename);
    read_image_file(filename, &row_pointers, &image, &width, &height);

    if (row_pointers != NULL) {
        printf("Converting PNG image to grayscale\n");
        convert_to_grayscale(row_pointers, &grayscale, width, height);
    } else if (image != NULL) {
        printf("Converting JPEG image to grayscale\n");
        convert_jpeg_to_grayscale(image, &grayscale, width, height);
        free(image);
    }

    // Apply kernel to the image

    // Create a Laplacian of Gaussian kernel
    double sigma = 2.0;  // Example sigma, adjust as needed
    double **lap_gaus = (double **)malloc(sizeof(double *) * KERNEL_SIZE);

    for (int i = 0; i < KERNEL_SIZE; i++) {
        lap_gaus[i] = (double *)malloc(sizeof(double) * KERNEL_SIZE);
    }

    lap_gaus_kernel(lap_gaus, KERNEL_SIZE, sigma);

    output = (double *)malloc(sizeof(double) * width * height);

    printf("Applying kernel to image\n");

    // (FOR DEBUGGING) Print first line of grayscale image
    // printf("Grayscale image:\n");
    // for (int i = 0; i < width; i++) {
    //     printf("%f ", grayscale[i + 20 * width]);
    // }
    // printf("\n");

    // Measure CPU execution time
    clock_t start_cpu = clock();
    apply_kernel_cpu(grayscale, output, width, height, lap_gaus);
    clock_t end_cpu = clock();
    double cpu_time_used = ((double) (end_cpu - start_cpu)) / CLOCKS_PER_SEC;
    printf("CPU execution time: %f seconds\n", cpu_time_used);

    printf("Writing CPU image to cpu_output.png\n");
    write_png_file("cpu_output.png", output, width, height);

    // // (FOR DEBUGGING) Print first line of output image
    // printf("CPU output:\n");
    // for (int i = 0; i < width; i++) {
    //     printf("%f ", output[i + 20 * width]);
    // }
    // printf("\n");

    // clean up the output
    memset(output, 0, sizeof(double) * width * height);

    // Measure CUDA execution time
    clock_t start_cuda = clock();
    apply_kernel_cuda_wrapper(grayscale, output, width, height, lap_gaus, 32);
    clock_t end_cuda = clock();
    double cuda_time_used = ((double) (end_cuda - start_cuda)) / CLOCKS_PER_SEC;
    printf("CUDA execution time: %f seconds\n", cuda_time_used);

    // // (FOR DEBUGGING) Print first line of output image
    // printf("CUDA output:\n");
    // for (int i = 0; i < width; i++) {
    //     printf("%f ", output[i + 20 * width]);
    // }
    // printf("\n");

    printf("Writing image to cuda_output.png\n");
    write_png_file("cuda_output.png", output, width, height);

    // Free memory
    if (row_pointers != NULL) {
        for (int y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);
    }
    free(grayscale);
    free(output);
    for (int i = 0; i < KERNEL_SIZE; i++) {
        free(lap_gaus[i]);
    }
    free(lap_gaus);

    return 0;
}