cmake_minimum_required(VERSION 2.8.7)
project(Raytracer)

# Glob all the source files 
file(GLOB SourceFiles "src/*.cpp" "src/*.hpp" "src/*.h")
file(GLOB TestFiles "test/*.cpp" "test/*.hpp" "src/*.hpp")

# Library output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin )


# Executables
add_executable(raytracer ${SourceFiles})
add_executable(raytracer-test ${TestFiles})
