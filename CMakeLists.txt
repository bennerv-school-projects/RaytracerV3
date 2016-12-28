cmake_minimum_required(VERSION 2.8.7)
project(raytracer)

add_executable(raytracer src/Main.cpp)


# Set the output to the bin directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Library output
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
