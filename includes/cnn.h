#ifndef CNN_H
#define CNN_H

#define KERNEL_SIZE 17

void apply_kernel_cpu(double *grayscale, double *output, int width, int height, double **kernel);
void normalize_array(double *array, int width, int height);

#ifdef __cplusplus
extern "C" {
#endif

void apply_kernel_cuda_wrapper(double *grayscale, double *output, int width, int height, double **kernel, int block_size);

#ifdef __cplusplus
}
#endif

#endif // CNN_H