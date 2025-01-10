cmake_minimum_required(VERSION 3.10)

# Set the project name
project(CNNProject LANGUAGES C CXX CUDA)

# Specify the C standard
set(CMAKE_C_STANDARD 17)

# Specify the CUDA architectures
set(CMAKE_CUDA_ARCHITECTURES 52 60 61 70 75 80)

# Add the executable
add_executable(CNNProject src/main.c src/conv.c src/conv.cu src/image.c)

# Include the header files directory
include_directories(includes)

# Find the PNG and JPEG libraries
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)

# Include the PNG and JPEG directories
include_directories(${PNG_INCLUDE_DIRS} ${JPEG_INCLUDE_DIRS})

# Link the PNG and JPEG libraries
target_link_libraries(CNNProject ${PNG_LIBRARIES} ${JPEG_LIBRARIES})