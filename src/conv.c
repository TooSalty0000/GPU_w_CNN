#include <stdio.h>
#include <limits.h>
#include "cnn.h"

void apply_kernel_cpu(double *grayscale, double *output, int width, int height, double **kernel) {
    if (!grayscale || !output) {
        fprintf(stderr, "Null pointer passed to apply_kernel\n");
        return;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double sum = 0.0;
            for (int ky = 0; ky < KERNEL_SIZE; ky++) {
                for (int kx = 0; kx < KERNEL_SIZE; kx++) {
                    int px = x + kx - KERNEL_SIZE / 2;
                    int py = y + ky - KERNEL_SIZE / 2;
                    if (px >= 0 && px < width && py >= 0 && py < height) {
                        sum += grayscale[py * width + px] * kernel[ky][kx];
                    }
                }
            }
            output[y * width + x] = sum;
        }
    }
}

void normalize_output(double *output, int width, int height) {
    double min_val = (double)INT_MAX;
    double max_val = (double)INT_MIN;

    // Find the minimum and maximum values in the output array
    for (int i = 0; i < width * height; i++) {
        if (output[i] < min_val) {
            min_val = output[i];
        }
        if (output[i] > max_val) {
            max_val = output[i];
        }
    }

    // Normalize the values to the range 0 to 255
    for (int i = 0; i < width * height; i++) {
        output[i] = (output[i] - min_val) * 255 / (max_val - min_val);
    }
}