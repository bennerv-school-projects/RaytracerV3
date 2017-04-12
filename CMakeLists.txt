cmake_minimum_required(VERSION 2.8.12)
project(Raytracer)

# Grab all the source files
aux_source_directory(./src SRC)

# Executables
add_executable(raytracer ${SRC})

# Include inih library
include_directories(${CMAKE_BINARY_DIR}/lib/inih ${CMAKE_BINARY_DIR}/lib/tinyxml2)

# Must build static/shared library prior to running cmake
if(WIN32) 
	target_link_libraries(raytracer ${CMAKE_BINARY_DIR}/lib/tinyxml2/Debug/tinyxml2.lib)
	target_link_libraries(raytracer ${CMAKE_BINARY_DIR}/lib/pthread/pthreadVC2.lib)
	include_directories(${CMAKE_BINARY_DIR}/lib/pthread)
else()
	target_link_libraries(raytracer ${CMAKE_BINARY_DIR}/lib/tinyxml2/libtinyxml2.a)

	# Threading library
	find_package(Threads REQUIRED)
	if(THREADS_HAVE_PTHREAD_ARG)
  		set_property(TARGET raytracer PROPERTY COMPILE_OPTIONS "-pthread")
  		set_property(TARGET raytracer PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
	endif()
	if(CMAKE_THREAD_LIBS_INIT)
  		target_link_libraries(raytracer "${CMAKE_THREAD_LIBS_INIT}")
	endif()
endif()