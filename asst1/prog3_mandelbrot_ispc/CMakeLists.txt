cmake_minimum_required(VERSION 3.10)
project(MandelbrotISPC)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")

# Include directories
include_directories(../common)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Find ISPC
find_program(ISPC_EXECUTABLE NAMES ispc PATHS ENV PATH)
if(NOT ISPC_EXECUTABLE)
    message(FATAL_ERROR "Could not find ISPC. Please install ISPC and add it to your PATH.")
endif()

# Custom command to compile ISPC files
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mandelbrot_ispc.h ${CMAKE_CURRENT_BINARY_DIR}/mandelbrot_ispc.o
    COMMAND ${ISPC_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/mandelbrot.ispc -o ${CMAKE_CURRENT_BINARY_DIR}/mandelbrot_ispc.o -h ${CMAKE_CURRENT_BINARY_DIR}/mandelbrot_ispc.h --target=avx2-i32x8
    DEPENDS mandelbrot.ispc
    COMMENT "Compiling mandelbrot.ispc"
)

# Source files
set(SOURCES
    main.cpp
    mandelbrotSerial.cpp
    ../common/ppm.cpp
)

# Add executable
add_executable(mandelbrot_ispc ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/mandelbrot_ispc.o)

# Link libraries
find_library(ISPCRT_LIBRARY NAMES ispcrt PATHS ${ISPC_EXECUTABLE}/../../lib)
target_link_libraries(mandelbrot_ispc pthread ${ISPCRT_LIBRARY})