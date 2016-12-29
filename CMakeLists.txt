cmake_minimum_required(VERSION 2.8.7)
project(Raytracer)

# Glob all the source files 
file(GLOB SourceFiles "src/*.cpp" "src/*.hpp" "src/*.h")

add_executable(raytracer ${SourceFiles})

# Set the output to the bin directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Library output
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
