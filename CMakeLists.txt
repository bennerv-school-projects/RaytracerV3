cmake_minimum_required(VERSION 2.8.7)
project(Raytracer)

# Include inih library
include_directories(${CMAKE_BINARY_DIR}/lib/inih ${CMAKE_BINARY_DIR}/lib/tinyxml2)

if(WIN32) 
	include_directories(${CMAKE_BINARY_DIR}/lib/pthread)
endif()

# Glob all the source files 
file(GLOB SourceFiles "src/*.cpp" "src/*.hpp" "src/*.h")
file(GLOB TestFiles "test/*.cpp" "test/*.hpp" "src/*.hpp")

# Program output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Executables
add_executable(raytracer ${SourceFiles})
add_executable(raytracer-test ${TestFiles})

# Must build static/shared library prior to running cmake
if(WIN32) 
	target_link_libraries(raytracer ${CMAKE_BINARY_DIR}/lib/tinyxml2/Debug/tinyxml2.lib)
else()
	target_link_libraries(raytracer ${CMAKE_BINARY_DIR}/lib/tinyxml2/libtinyxml2.a)
endif()