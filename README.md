# ConvProject

ConvProject is a C/CUDA application that reads an image file (PNG or JPEG), converts it to grayscale, applies a Laplacian of Gaussian kernel to the image, and outputs the processed image. The project demonstrates both CPU and GPU (CUDA) implementations of the kernel application.

## Project Structure

```
CNNProject/
├── .gitignore
├── .vscode/
│   ├── .prettierrc
│   ├── c_cpp_properties.json
│   └── settings.json
├── build/
│   ├── cmake_install.cmake
│   ├── CMakeCache.txt
│   ├── CMakeFiles/
│   ├── CNNProject
│   └── Makefile
├── CMakeLists.txt
├── includes/
│   ├── cnn.h
│   └── image.h
├── src/
│   ├── conv.c
│   ├── conv.cu
│   ├── image.c
│   └── main.c
```

## Requirements

- CMake 3.10 or higher
- GCC
- CUDA Toolkit
- libpng
- libjpeg

## Building the Project

1. Clone the repository:
    ```sh
    git clone <repository_url>
    cd CNNProject
    ```

2. Create a build directory and navigate into it:
    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to configure the project:
    ```sh
    cmake ..
    ```

4. Build the project:
    ```sh
    make
    ```

## Running the Project

To run the project, use the following command:
```sh
./ConvProject <image_filename>

