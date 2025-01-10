#ifndef IMAGE_H
#define IMAGE_H

void read_jpeg_file(const char *filename, unsigned char **image, int *width, int *height);
void write_png_file(const char *filename, double *grayscale, int width, int height);
void convert_to_grayscale(png_bytep *row_pointers, double **grayscale, int width, int height);
void read_png_file(const char *filename, png_bytep **row_pointers, int *width, int *height);
void read_image_file(const char *filename, png_bytep **row_pointers, unsigned char **image, int *width, int *height);
void convert_jpeg_to_grayscale(unsigned char *image, double **grayscale, int width, int height);

#endif // IMAGE_H