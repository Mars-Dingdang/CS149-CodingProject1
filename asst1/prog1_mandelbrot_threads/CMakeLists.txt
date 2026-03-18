cmake_minimum_required(VERSION 3.10)
project(MandelbrotThreads)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -fPIC")

# Include directories
include_directories(../common)

# Source files
set(SOURCES
    main.cpp
    mandelbrotSerial.cpp
    mandelbrotThread.cpp
    ../common/ppm.cpp
)

# Add executable
add_executable(mandelbrot ${SOURCES})

# Link libraries
target_link_libraries(mandelbrot pthread)