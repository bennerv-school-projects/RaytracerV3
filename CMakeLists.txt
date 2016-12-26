cmake_minimum_required(VERSION 2.8.9)
project(raytracer)

add_executable(raytracer src/Main.cpp)


# Set the output to the bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)